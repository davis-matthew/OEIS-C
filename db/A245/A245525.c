/*
 * A245525  Unique integer r with -prime(n)/2 < r <= prime(n)/2 such that p(n)
 *          == r (mod prime(n)), where p(.) is the partition function given by
 *          A000041.
 * https://oeis.org/A245525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 67, i16 n <= 6668, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245525: a(n) = p(n) mod prime(n), returned as the signed residue
   r with -prime(n)/2 < r ≤ prime(n)/2.  p(n) is the partition number. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, out of range handled safely */

    uint32_t prime = get_prime(n);        /* prime(n) */
    uint32_t *modp   = (uint32_t*)malloc((size_t)(n + 1) * sizeof(uint32_t));
    if (!modp) return (seq_t)0;           /* allocation failure – unlikely */

    modp[0] = 1 % prime;                  /* p(0) = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        int64_t acc = 0;
        /* generalized pentagonal numbers */
        for (int k = 1; ; ++k) {
            int64_t pent1 = (int64_t)k * ((int64_t)3 * k - 1) / 2;
            if (pent1 > (int64_t)m) break;          /* no further terms for this m */

            int sign = (k & 1) ? 1 : -1;            /* (-1)^(k+1) */
            acc += sign * (int64_t)modp[m - pent1];

            int64_t pent2 = (int64_t)k * ((int64_t)3 * k + 1) / 2;
            if (pent2 <= (int64_t)m)
                acc += sign * (int64_t)modp[m - pent2];
        }

        int64_t r = acc % prime;
        if (r < 0) r += prime;                    /* ensure 0 ≤ r < prime */
        modp[m] = (uint32_t)r;
    }

    uint32_t r_mod = modp[n];
    free(modp);

    int32_t signed_r;
    if (r_mod > prime / 2)
        signed_r = (int32_t)(r_mod - prime);
    else
        signed_r = (int32_t)r_mod;

    return (seq_t)signed_r;
}
