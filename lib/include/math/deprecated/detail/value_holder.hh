//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VALUE_HOLDER_HH
#define NEUTRINO_MATH_VALUE_HOLDER_HH

namespace neutrino::math::detail {
    /*
 * Passes lvalues and stores rvalues
 */
    template<typename T>
    class temp_value_holder;

    template<typename T>
    class temp_value_holder <T&> {
        private:
            T& ref;

        public:
            temp_value_holder(T& ref)
                : ref(ref) {
            }

            T& get() { return ref; }
            const T& get() const { return ref; }
    };

    template<typename T>
    class temp_value_holder <T&&> {
        private:
            T value;

        public:
            temp_value_holder(T&& ref)
                : value(std::move(ref)) {
            }

            T& get() { return value; }
            const T& get() const { return value; }
    };
}

#endif
