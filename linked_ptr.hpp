#include <type_traits>
#include <utility>

#ifndef _SMART_PTR_LINKED_PTR_HPP
#define _SMART_PTR_LINKED_PTR_HPP

namespace smart_ptr {

    namespace details {
        struct Connector {
            Connector *_conWith = nullptr;

            inline explicit operator bool() const noexcept {
                return _conWith != nullptr;
            }
        };
    }
    template<typename Type>
    class linked_ptr {
        template<typename _Type>
        friend
        class linked_ptr;

    private:
        details::Connector * _left = new details::Connector;
        details::Connector * _right = new details::Connector;
        Type *_ptr = nullptr;

        void clear() {
            if (unique()) {
                static_assert(sizeof(Type) > 0, "incomplete type" );
                delete _ptr;
            }
            if (_left->_conWith)
                _left->_conWith->_conWith = _right->_conWith;
            if (_right->_conWith)
                _right->_conWith->_conWith = _left->_conWith;
        }

        template<typename _Type>
        void copy(const linked_ptr<_Type> &l_ptr) {
            if (_ptr == l_ptr._ptr)
                return;

            clear();

            _ptr = l_ptr._ptr;

            _right->_conWith = l_ptr._right->_conWith;
            if (_right->_conWith)
                _right->_conWith->_conWith = _right;
            _left->_conWith = l_ptr._right;
            l_ptr._right->_conWith = _left;
        }

    public:
        constexpr linked_ptr(std::nullptr_t) : linked_ptr() {}

        constexpr linked_ptr() noexcept {}

        template<
                typename _Type,
                typename = std::enable_if_t<
                        std::is_convertible_v<_Type *, Type *>
                >
        >
        explicit linked_ptr(_Type *ptr) {
            _ptr = ptr;
        }

        template<
                typename _Type,
                typename = std::enable_if_t<
                        std::is_convertible_v<_Type *, Type *>
                >
        >
        linked_ptr(linked_ptr<_Type> &l_ptr) noexcept {
            copy(l_ptr);
        }

        ~linked_ptr() {
            clear();
            delete _left;
            delete _right;
        }

        void reset(Type *ptr = nullptr) noexcept {
            clear();
            _ptr = ptr;
            _left->_conWith = _right->_conWith = nullptr;
        }

        Type *get() const noexcept {
            return _ptr;
        }

        void swap(linked_ptr<Type> &l_ptr) noexcept {
            std::swap(_ptr, l_ptr._ptr);

            std::swap(_left, l_ptr._left);
            std::swap(_right, l_ptr._right);
        }

        bool unique() const noexcept {
            return (_ptr && !_left->_conWith && !_right->_conWith);
        }

        template<typename _Type>
        inline bool operator==(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr == l_ptr._ptr);
        }

        template<typename _Type>
        inline bool operator!=(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr != l_ptr._ptr);
        }

        template<typename _Type>
        inline bool operator<(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr < l_ptr._ptr);
        }

        template<typename _Type>
        inline bool operator>(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr > l_ptr._ptr);
        }

        template<typename _Type>
        inline bool operator<=(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr <= l_ptr._ptr);
        }

        template<typename _Type>
        inline bool operator>=(const linked_ptr<_Type> &l_ptr) const noexcept {
            return (_ptr >= l_ptr._ptr);
        }

        template<
                typename _Type,
                typename = std::enable_if_t<
                        std::is_convertible_v<_Type *, Type *>
                >
        >
        linked_ptr<Type>& operator=(linked_ptr<_Type> &l_ptr) noexcept {
            copy(l_ptr);
            return *this;
        }

        template<
                typename _Type,
                typename = std::enable_if_t<
                        std::is_convertible_v<_Type *, Type *>
                >
        >
        linked_ptr<Type>& operator=(const linked_ptr<_Type> &l_ptr) noexcept {
            copy(l_ptr);
            return *this;
        }

        Type &operator*() const noexcept {
            return *_ptr;
        }

        Type *operator->() const noexcept {
            return _ptr;
        }

        inline explicit operator bool() const noexcept {
            return (_ptr != nullptr);
        }
    };
}

#endif //_SMART_PTR_LINKED_PTR_HPP

