
#ifndef CPP_LINKED_PTR_LINKED_PTR_H
#define CPP_LINKED_PTR_LINKED_PTR_H

#include <iostream>

namespace smart_ptr {
    namespace {
        struct linked_ptr_base {
            mutable linked_ptr_base * prev;
            mutable linked_ptr_base * next;
        };
    }

    template <typename T>
    struct linked_ptr : linked_ptr_base {
    private:
        template <typename U> friend class linked_ptr;

        T *value;

        template <typename U>
        void make_link(const linked_ptr<U> &ptr) {
            this->prev = ptr.prev;
            ptr.prev = this;
            this->next = const_cast<linked_ptr<U>*>(&ptr);
            this->prev->next = this;
        }

    public:
        linked_ptr() {
            prev = this;
            next = this;
            value = nullptr;
        }

        template <typename U>
        explicit linked_ptr(U* value){
            prev = this;
            next = this;
            this->value = value;
        }

        linked_ptr(const linked_ptr<T>& other){
            value = other.value;
            prev = next = this;
            if (value) {
                make_link(other);
            }
        }

        template <typename U>
        linked_ptr(const linked_ptr<U>& other){
            value = other.value;
            prev = next = this;
            if (value) {
                make_link(other);
            }
        }

        linked_ptr& operator=(const linked_ptr& ptr) {
            linked_ptr tmp = ptr;
            swap(tmp, *this);
            return *this;
        }

        template <typename U>
        linked_ptr& operator=(const linked_ptr<U>& ptr) {
            linked_ptr tmp = ptr;
            swap(tmp, *this);
            return *this;
        }

        template <typename U>
        linked_ptr& operator=(U* ptr) {
            reset(ptr);
            return *this;
        }

        ~linked_ptr() {
            reset();
        }


        void reset() {
            reset(static_cast<T*>(nullptr));
        }

        template <typename U>
        void reset(U* ptr) {
            if (ptr != value) {
                if (!unique()) {
                    next->prev = prev;
                    prev->next = next;
                    prev = this;
                    next = this;
                } else {
                    delete value;
                }
                value = ptr;
            }
        }

        bool unique() const {
            return (next == static_cast<const linked_ptr_base*>(this));
        }


        template <typename U>
        void swap(linked_ptr<U>& other) {
            if (value != other.value) {
                std::swap(value, other.value);
                if (!unique()) {
                    if (!other.unique()) {
                        std::swap(other.next->prev, this->next->prev);
                        std::swap(other.prev->next, this->prev->next);
                        std::swap(other.next, this->next);
                        std::swap(other.prev, this->prev);
                    } else {
                        other.next = this->next;
                        other.prev = this->prev;
                        this->next->prev = &other;
                        this->prev->next = &other;
                        next = this;
                        prev = this;
                    }
                } else {
                    if (!other.unique()) {
                        this->next = other.next;
                        this->prev = other.prev;
                        other.next->prev = this;
                        other.prev->next = this;
                        other.next = this;
                        other.prev = this;
                    }
                }
            }
        }

        T& operator*() const {
            return *value;
        }

        T* operator->() const {
            return value;
        }

        T* get() const {
            return value;
        }

        explicit operator bool() const {
            return value != nullptr;
        }

        bool operator!() {
            return (value == nullptr);
        }
    };

    template <typename T, typename U>
    inline bool operator==(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() == b.get());
    }

    template <typename T, typename U>
    inline bool operator!=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() != b.get());
    }

    template <typename T, typename U>
    inline bool operator<(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() < b.get());
    }

    template <typename T, typename U>
    inline bool operator<=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() <= b.get());
    }

    template <typename T, typename U>
    inline bool operator>(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() > b.get());
    }

    template <typename T, typename U>
    inline bool operator>=(const linked_ptr<T>& a, const linked_ptr<U>& b) {
        return (a.get() >= b.get());
    }

    template <typename U>
    linked_ptr<U> make_linked_ptr(U* ptr) {
        return linked_ptr<U>(ptr);
    }

}

#endif //CPP_LINKED_PTR_LINKED_PTR_H