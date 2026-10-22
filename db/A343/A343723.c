/*
 * A343723  a(n) is 1/4 of the number of starting residues r modulo n from
 *          which repeated iterations of the mapping r -> r^2 mod n never
 *          reach a fixed point.
 * https://oeis.org/A343723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n - ∏_{p^e‖n} (p^(e-1) + 2^{v2(p-1)}) ) / 4 where v2(x)
 *            = exponent of 2 in x. For p = 2 we use s = 2^e
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work in 64‑bit integer to avoid overflow in intermediate
       steps; the final result fits in seq_t. */
    uint64_t N = (uint64_t)n;
    uint64_t prod_s = 1;

    idx_t m = n;                /* copy of n for factorisation */
    for (idx_t p = 2; (uint64_t)p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        /* exponent of p in n */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        uint64_t s;
        if (p == 2) {
            /* all residues modulo 2^e reach a fixed point */
            s = 1ULL << e;
        } else {
            /* odd prime power */
            /* p^(e-1) */
            uint64_t pw = 1;
            for (idx_t i = 1; i < e; ++i)
                pw *= (uint64_t)p;

            /* v2(p-1) */
            int v2 = 0;
            uint64_t tmp = (uint64_t)p - 1;
            while ((tmp & 1) == 0) {
                ++v2;
                tmp >>= 1;
            }
            uint64_t two_pow = 1ULL << v2;

            s = pw + two_pow;
        }

        prod_s *= s;
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        idx_t p = m;
        idx_t e = 1;

        uint64_t s;
        if (p == 2) {
            s = 2;
        } else {
            /* p^(e-1) = 1 for e=1 */
            int v2 = 0;
            uint64_t tmp = (uint64_t)p - 1;
            while ((tmp & 1) == 0) {
                ++v2;
                tmp >>= 1;
            }
            uint64_t two_pow = 1ULL << v2;
            s = 1 + two_pow;   /* since p^(e-1) = 1 */
        }
        prod_s *= s;
    }

    uint64_t A = N - prod_s;        /* A = N - ∏ s_p */
    uint64_t res = A / 4;
    return (seq_t)res;
}
