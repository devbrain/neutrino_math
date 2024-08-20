//
// Created by igor on 8/14/24.
//

#ifndef NEUTRINO_MATH_MATRIX_HH
#define NEUTRINO_MATH_MATRIX_HH

#include <math/vector.hh>
#include <math/detail/matrix_ops.hh>
#include <math/detail/matrix_storage.hh>
#include <math/detail/matrix_access.hh>

namespace neutrino::math {
    namespace detail {
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class matrix_iterator_proxy;

        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class const_matrix_iterator_proxy;
    }

    template<typename E, std::size_t R, std::size_t C>
    class matrix : public matrix_ops <E, R, C> {
        public:
            using storage_t = detail::matrix_storage <E, R, C>;
            using row_t = typename storage_t::row_t;
            typename storage_t::values_t values{};

        public:
            matrix() = default;

            constexpr matrix(const E (& data)[R][C])
                : values{storage_t::template create(data, std::make_integer_sequence <int, R>{})} {
            }

            auto operator()(std::size_t r, std::size_t c) const {
                if (r >= R) {
                    throw std::out_of_range("matrix: Index out of range");
                }
                return values[r][c];
            }

            template<typename T, class = std::enable_if_t <is_matrix_or_matrix_exp_v <T, R, C>>>
            constexpr matrix(const T& v) {
                for (std::size_t r = 0; r < R; r++) {
                    for (std::size_t c = 0; c < C; c++) {
                        values[r][c] = v(r, c);
                    }
                }
            }

            template<bool ByRow>
            matrix& operator =(const detail::matrix_axis_access <E, R, C, ByRow>& view) {
                for (std::size_t i = 0; i < view.size(); i++) {
                    if constexpr (ByRow) {
                        values[view.get_index()][i] = view[i];
                    } else {
                        values[i][view.get_index()] = view[i];
                    }
                }
                return *this;
            }

            template<typename T, class = std::enable_if_t <
                         is_matrix_or_matrix_exp_v <T, R, C>>>
            constexpr matrix& operator =(const T& v) {
                if ((void*)&v != (void*)this) {
                    for (std::size_t r = 0; r < R; r++) {
                        for (std::size_t c = 0; c < C; c++) {
                            values[r][c] = v(r, c);
                        }
                    }
                }
                return *this;
            }

            [[nodiscard]] constexpr std::size_t get_rows_num() const {
                return R;
            }

            [[nodiscard]] constexpr std::size_t get_columns_num() const {
                return C;
            }

            detail::const_matrix_iterator_proxy <E, R, C, true> const_rows() const;
            detail::const_matrix_iterator_proxy <E, R, C, false> const_columns() const;

            detail::const_matrix_iterator_proxy <E, R, C, true> rows() const;
            detail::const_matrix_iterator_proxy <E, R, C, false> columns() const;

            detail::matrix_iterator_proxy <E, R, C, true> rows();
            detail::matrix_iterator_proxy <E, R, C, false> columns();
    };

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
        class matrix_iterator_proxy {
            friend class matrix <E, R, C>;

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
            friend class matrix <E, R, C>;

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
    detail::const_matrix_iterator_proxy <E, R, C, true> matrix <E, R, C>::const_rows() const {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    detail::const_matrix_iterator_proxy <E, R, C, false> matrix <E, R, C>::const_columns() const {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    detail::const_matrix_iterator_proxy <E, R, C, true> matrix <E, R, C>::rows() const {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    detail::const_matrix_iterator_proxy <E, R, C, false> matrix <E, R, C>::columns() const {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    detail::matrix_iterator_proxy <E, R, C, true> matrix <E, R, C>::rows() {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    detail::matrix_iterator_proxy <E, R, C, false> matrix <E, R, C>::columns() {
        return {*this};
    }

    template<typename E, std::size_t R, std::size_t C>
    auto make_matrix(const E (&& data)[R][C]) {
        return matrix <E, R, C>{std::forward <decltype(data)>(data)};
    }
}

#endif
