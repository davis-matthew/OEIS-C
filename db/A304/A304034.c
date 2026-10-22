/*
 * A304034  Number of ways to write n as p + 2^k + (1+(n mod 2))*3^m with p
 *          prime, where k and m are positive integers with 2^k + (1+(n mod
 *          2))*3^m squarefree.
 * https://oeis.org/A304034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (p,k,m) : p prime, k>0, m>0, n = p + 2^k + (1+(n mod
 *            2))*3^m, * 2^k + (1+(n mod 2))*3^m is squarefree }
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 4) return 0;                       /* early exit, no representations */
    idx_t t_mult = 1 + (n & 1u);                /* 1 for even n, 2 for odd n */

    for (idx_t k = 1; ; ++k)
    {
        seq_t pow2k = pow2(k);                  /* 2^k */

        if (pow2k >= n) break;                 /* remaining space for p+t is non‑positive */

        idx_t remaining = (idx_t)(n - pow2k);   /* n - 2^k */

        if (remaining <= 2) continue;          /* need at least 2 for prime p */

        idx_t max_t = remaining - 2;           /* max possible t so that p>=2 */

        for (idx_t m = 1; ; ++m)
        {
            seq_t pow3m = pow3(m);              /* 3^m */
            seq_t t = (seq_t)t_mult * pow3m;    /* (1+(n mod 2))*3^m */

            if (t > (seq_t)max_t) break;        /* further m will only increase t */

            seq_t p = n - pow2k - t;           /* candidate prime */
            if (p < 2) continue;               /* safety check */

            if (!is_prime((idx_t)p)) continue; /* p must be prime */

            seq_t sq = pow2k + t;
            if (!is_squarefree(sq)) continue;  /* 2^k + t must be squarefree */

            ++count;                           /* valid representation found */
        }
    }

    return count;
}
