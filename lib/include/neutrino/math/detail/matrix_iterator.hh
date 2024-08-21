//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_ITERATOR_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_ITERATOR_HH

#include <neutrino/math/matrix.hh>

namespace neutrino::math {
    template<typename E, std::size_t R, std::size_t C, bool ByRow>
    class matrix_iterator {
        friend class detail::matrix_iterator_proxy <E, R, C, ByRow>;

        public:
            using value_type = detail::matrix_axis_access <E, R, C, ByRow>;
            // Prefix increment
            matrix_iterator& operator++() {
                axis++;
                return *this;
            }

            // Postfix increment
            matrix_iterator operator++(int) {
                matrix_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            auto operator*() const { return value_type{owner, axis}; }

            friend bool operator==(const matrix_iterator& a, const matrix_iterator& b) { return a.axis == b.axis; };
            friend bool operator!=(const matrix_iterator& a, const matrix_iterator& b) { return a.axis != b.axis; };

            [[nodiscard]] std::size_t get_index() const noexcept {
                return axis;
            }

        private:
            matrix_iterator(std::size_t row_, matrix <E, R, C>& owner_)
                : axis(row_),
                  owner(owner_) {
            }

            std::size_t axis;
            matrix <E, R, C>& owner;
    };

    template<typename E, std::size_t R, std::size_t C, bool ByRow>
    class const_matrix_iterator {
        friend class detail::const_matrix_iterator_proxy <E, R, C, ByRow>;

        public:
            using value_type = detail::const_matrix_axis_access <E, R, C, ByRow>;
            // Prefix increment
            const_matrix_iterator& operator++() {
                axis++;
                return *this;
            }

            // Postfix increment
            const_matrix_iterator operator++(int) {
                const_matrix_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            auto operator*() const { return value_type{owner, axis}; }

            friend bool operator==(const const_matrix_iterator& a, const const_matrix_iterator& b) {
                return a.axis == b.axis;
            };

            friend bool operator!=(const const_matrix_iterator& a, const const_matrix_iterator& b) {
                return a.axis != b.axis;
            };

            [[nodiscard]] std::size_t get_index() const noexcept {
                return axis;
            }

        private:
            const_matrix_iterator(std::size_t row_, const matrix <E, R, C>& owner_)
                : axis(row_),
                  owner(owner_) {
            }

            std::size_t axis;
            const matrix <E, R, C>& owner;
    };

    namespace detail {
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class matrix_iterator_proxy;
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class const_matrix_iterator_proxy;
    }
    // forward declarations
    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::matrix_iterator_proxy<E, R, C, false> columns(matrix <E, R, C>& m);

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::const_matrix_iterator_proxy<E, R, C, false> columns(const matrix <E, R, C>& m);

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::matrix_iterator_proxy<E, R, C, true> rows(matrix <E, R, C>& m);

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::const_matrix_iterator_proxy<E, R, C, true> rows(const matrix <E, R, C>& m);
    // --
    namespace detail {
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class matrix_iterator_proxy {
            template<typename EE, std::size_t RR, std::size_t CC>
            friend constexpr matrix_iterator_proxy<EE, RR, CC, ByRow> math::columns(matrix <EE, RR, CC>& m);

            template<typename EE, std::size_t RR, std::size_t CC>
            friend constexpr matrix_iterator_proxy<EE, RR, CC, ByRow> math::rows(matrix <EE, RR, CC>& m);

            public:
                matrix_iterator <E, R, C, ByRow> begin() const {
                    return {0, owner};
                }

                matrix_iterator <E, R, C, ByRow> end() const {
                    return {ByRow ? C : R, owner};
                }

            private:
                matrix_iterator_proxy(matrix <E, R, C>& owner_)
                    : owner(owner_) {
                }

                matrix <E, R, C>& owner;
        };

        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class const_matrix_iterator_proxy {
            template<typename EE, std::size_t RR, std::size_t CC>
            friend constexpr const_matrix_iterator_proxy<EE, RR, CC, ByRow> math::columns(const matrix <EE, RR, CC>& m);

            template<typename EE, std::size_t RR, std::size_t CC>
            friend constexpr const_matrix_iterator_proxy<EE, RR, CC, ByRow> math::rows(const matrix <EE, RR, CC>& m);

            public:
                const_matrix_iterator <E, R, C, ByRow> begin() const {
                    return {0, owner};
                }

                const_matrix_iterator <E, R, C, ByRow> end() const {
                    return {ByRow ? C : R, owner};
                }

            private:
                const_matrix_iterator_proxy(const matrix <E, R, C>& owner_)
                    : owner(owner_) {
                }

                const matrix <E, R, C>& owner;
        };
    }

    template<typename E, std::size_t R, std::size_t C>
    using rows_iterator = matrix_iterator <E, R, C, true>;

    template<typename E, std::size_t R, std::size_t C>
    using columns_iterator = matrix_iterator <E, R, C, false>;

    template<typename E, std::size_t R, std::size_t C>
    using const_rows_iterator = const_matrix_iterator <E, R, C, true>;

    template<typename E, std::size_t R, std::size_t C>
    using const_columns_iterator = const_matrix_iterator <E, R, C, false>;

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::matrix_iterator_proxy<E, R, C, false> columns(matrix <E, R, C>& m) {
        return {m};
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::const_matrix_iterator_proxy<E, R, C, false> columns(const matrix <E, R, C>& m) {
        return {m};
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::matrix_iterator_proxy<E, R, C, true> rows(matrix <E, R, C>& m) {
        return {m};
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr detail::const_matrix_iterator_proxy<E, R, C, true> rows(const matrix <E, R, C>& m) {
        return {m};
    }


}

#endif
