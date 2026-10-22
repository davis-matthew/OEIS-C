/*
 * A373374  a(n) = 1 if both A001414(n) and A003415(n) are even, otherwise 0,
 *          where A001414 is the sum of prime factors with repetition, and
 *          A003415 is the arithmetic derivative.
 * https://oeis.org/A373374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if both A001414(n) (sum of prime factors with
 *            repetition) and A003415(n) (arithmetic derivative) are even;
 *            otherwise 0. We compute the parities directly during prime
 *            factorisation
 * Range:     u8 n <= 9404, u16 n <= 9404, u32 n <= 9404, u64 n <= 9404,
 *            u128 n <= 9404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence value type and index type are defined by the build. */
seq_t a(idx_t n)
{
    /* Convert the index to a native integer that fits in 64 bits.
       We assume all n that occur in tests fit in uint64_t.
    */
    uint64_t v = (uint64_t)n;
    uint64_t temp = v;

    /* Parities: 0=even, 1=odd */
    unsigned int sum_pf_parity   = 0;   /* parity of sum of prime factors (with multiplicity) */
    unsigned int deriv_parity    = 0;   /* parity of arithmetic derivative */

    /* First factor out powers of 2 */
    int e2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e2;
    }

    if (e2 > 0) {
        /* For p = 2
           • p is even => product e*p is even => no effect on sum_pf_parity
           • derivative term parity:
              * a_parity = (v / 2) parity
              * e_parity = e2 parity
              * pow_parity = 0 if e2-1 >= 1 (i.e. e2 >= 2) else 1
              * term_parity = a_parity & e_parity & pow_parity
        */
        unsigned int a_parity = ((v / 2ULL) & 1ULL) ? 1 : 0;
        unsigned int e_parity = (e2 & 1) ? 1 : 0;
        unsigned int pow_parity = (e2 >= 2) ? 0 : 1;   /* 2^(e2-1) parity */
        unsigned int term_parity = a_parity & e_parity & pow_parity;
        deriv_parity ^= term_parity;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* sum of prime factors parity */
            if ((p & 1ULL) && (e & 1)) {
                sum_pf_parity ^= 1;
            }

            /* derivative term parity */
            unsigned int a_parity   = ((v / p) & 1ULL) ? 1 : 0;
            unsigned int e_parity   = (e & 1) ? 1 : 0;
            unsigned int pow_parity = 1;                 /* p odd => any power odd */
            unsigned int term_parity = a_parity & e_parity & pow_parity;
            deriv_parity ^= term_parity;
        }
    }

    /* Any remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        uint64_t p = temp;
        int e = 1;   /* exponent is 1 */

        /* sum of prime factors parity */
        if ((p & 1ULL) && (e & 1)) {
            sum_pf_parity ^= 1;
        }

        /* derivative term parity */
        unsigned int a_parity   = ((v / p) & 1ULL) ? 1 : 0;
        unsigned int e_parity   = 1;               /* e == 1 */
        unsigned int pow_parity = 1;               /* p odd implies odd */
        unsigned int term_parity = a_parity & e_parity & pow_parity;
        deriv_parity ^= term_parity;
    }

    /* Result is 1 if both parities are zero (even), otherwise 0. */
    return (sum_pf_parity == 0 && deriv_parity == 0) ? (seq_t)1 : (seq_t)0;
}
