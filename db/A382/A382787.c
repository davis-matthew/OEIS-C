/*
 * A382787  The product of exponents in the prime factorization of the numbers
 *          whose prime factorization contains exponents that are either 1 or
 *          even.
 * https://oeis.org/A382787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = product of exponents in the prime factorization of the
    nth integer whose prime exponents are either 1 or even
    (i.e. no exponent >1 is odd). The sequence is thus listed in
    increasing order of the qualifying integers.   */
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    for (;;) {
        ++m;                          /* examine integer m */
        idx_t temp = m;
        seq_t prod = 1;
        bool bad = false;

        for (idx_t i = 2; i <= temp / i; ++i) {
            if (temp % i == 0) {
                idx_t exp = 0;
                while (temp % i == 0) {
                    temp /= i;
                    ++exp;
                }
                if ((exp & 1) && exp > 1) { /* odd exponent >1 */
                    bad = true;
                    break;
                }
                if (exp > 1)                /* multiply by the even exponent */
                    prod *= (seq_t)exp;
            }
        }
        if (bad)
            continue;                          /* m does not qualify */

        /* remaining factor >1 has exponent 1, which does not change prod */
        ++count;
        if (count == n)
            return prod;
    }
}
