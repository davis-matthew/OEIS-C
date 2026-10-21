/*
 * A271554  a(n) = G_n(7), where G is the Goodstein function defined in
 *          A266201.
 * https://oeis.org/A271554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = G_n(7), where G_0(7)=7 and G_{k+1}(7)=B_{k+2}(G_k(7))-1.
 *            B_{b} replaces all occurrences of hereditary base b within a
 *            number by b+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 15, u64 n <= 425,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* recursive hereditary base change: replace base 'base' with base+1 */
static seq_t B_helper(seq_t base, seq_t x)
{
    if (x < base)
        return x;                    /* nothing to replace */

    /* find the largest exponent e such that base^e <= x */
    seq_t pow = 1;
    seq_t e = 0;
    while (pow <= x / base) {
        pow *= base;
        ++e;
    }

    seq_t c    = x / pow;   /* 1 <= c < base */
    seq_t rest = x % pow;   /* 0 <= rest < pow */

    seq_t e_prime    = B_helper(base, e);
    seq_t rest_prime = B_helper(base, rest);

    seq_t term = big_pow(base + (seq_t)1, e_prime);
    term *= c;                     /* multiply by the coefficient */

    return term + rest_prime;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)7;           /* G_0(7) */
    for (idx_t step = 1; step <= n; ++step) {
        /* G_step(7) = B_{step+1}(G_{step-1}(7)) - 1 */
        val = B_helper(step + (idx_t)1, val);
        val = val - (seq_t)1;
    }
    return val;
}
