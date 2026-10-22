/*
 * A381212  a(n) is the smallest element of the set of bases and exponents
 *          (including exponents = 1) in the prime factorization of n.
 * https://oeis.org/A381212
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest element of the set of bases and exponents in the
 *            prime factorisation of n (including exponents = 1). The function
 *            returns 1 if any exponent equals 1, otherwise it returns the
 *            minimal value among all prime bases and all exponents
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 2) {
        /* Offset starts at n = 2; behaviour below is irrelevant. */
        return (seq_t)0;
    }

    uint64_t m = (uint64_t)n;          /* n fits in 64 bits for all inputs */
    bool exp_one_found = false;        /* true if any exponent equals 1 */
    bool smallest_set = false;         /* true when 'smallest' has a value */
    seq_t smallest = 0;                /* will hold the minimum value found */

    /* Factor out 2 separately */
    if ((m & 1ULL) == 0) {
        uint64_t p = 2;
        uint64_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if (!smallest_set || (seq_t)p < smallest) {
            smallest = (seq_t)p;
            smallest_set = true;
        }
        if (e == 1) {
            exp_one_found = true;
        }
        if (!smallest_set || (seq_t)e < smallest) {
            smallest = (seq_t)e;
            smallest_set = true;
        }
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (!smallest_set || (seq_t)p < smallest) {
                smallest = (seq_t)p;
                smallest_set = true;
            }
            if (e == 1) {
                exp_one_found = true;
            }
            if (!smallest_set || (seq_t)e < smallest) {
                smallest = (seq_t)e;
                smallest_set = true;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        uint64_t p = m;
        if (!smallest_set || (seq_t)p < smallest) {
            smallest = (seq_t)p;
            smallest_set = true;
        }
        /* This prime occurs with exponent 1 */
        exp_one_found = true;
    }

    return exp_one_found ? (seq_t)1 : smallest;
}
