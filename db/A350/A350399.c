/*
 * A350399  a(n) is the number of prime pairs (p,q) with p <= q, p+q = 2*n,
 *          and p*q mod (2*n) prime.
 * https://oeis.org/A350399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime pairs (p,q) with p <= q, * p + q = 2*n,
 *            and (p * q) mod (2*n) is prime
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* n is at least 1 */
    const idx_t m = 2 * n;          /* 2*n fits in idx_t */
    seq_t    cnt = 0;               /* result in seq_t */

    /* iterate p from 2 to m/2 inclusive so that p <= q = m - p */
    for (idx_t p = 2; p <= m / 2; ++p)
    {
        if (!is_prime_small((seq_t)p))
            continue;                 /* p not prime */

        const idx_t q = m - p;        /* q >= p by construction */

        if (!is_prime_small((seq_t)q))
            continue;                 /* q not prime */

        /* compute (p*q) mod m; product fits in 64‑bit for m <= 20000,
         * but we use seq_t to be generic. */
        const seq_t prod_mod = ((seq_t)p * (seq_t)q) % (seq_t)m;

        if (is_prime_small(prod_mod))
            ++cnt;
    }

    return cnt;
}
