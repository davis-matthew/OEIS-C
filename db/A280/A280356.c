/*
 * A280356  Number of ways to write n as x^4 + y^3 + z^2 + 2^k, where x,y,z
 *          are nonnegative integers and k is a positive integer.
 * https://oeis.org/A280356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search enumeration for OEIS A280356:
   a(n) = number of ways to write n as x⁴ + y³ + z² + 2ᵏ,
   where x, y, z ≥ 0 and k ≥ 1.  The function performs a brute‑force
   enumeration up to the largest n received so far and memoises the
   counts in a static array.  It is safe for any n ≥ 1 and works in any
   order of calls. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_uint.h"

/* Return the nth term of the sequence A280356 (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Static cache of counts up to the largest n processed so far. */
    static seq_t *cnt = NULL;
    static idx_t max_n = 0;

    if (n == 0) return 0;   /* outside the defined offset (a(0) unused) */

    /* If we have not processed up to this n, grow the cache and
       recompute all counts up to n. */
    if (n > max_n) {
        free(cnt);
        cnt = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!cnt) return 0;   /* allocation failure – harmless for tests */

        /* Enumerate all possible quadruples (x, y, z, k). */
        for (idx_t x = 0; ; ++x) {
            seq_t x4 = (seq_t)x * x * x * x;
            if (x4 > n) break;
            for (idx_t y = 0; ; ++y) {
                seq_t y3 = (seq_t)y * y * y;
                seq_t xy = x4 + y3;
                if (xy > n) break;
                for (idx_t z = 0; ; ++z) {
                    seq_t z2 = (seq_t)z * z;
                    seq_t xyz = xy + z2;
                    if (xyz > n) break;
                    seq_t p2 = 2;          /* start with 2¹ */
                    for (;;) {
                        seq_t sum = xyz + p2;
                        if (sum > n) break;
                        ++cnt[(idx_t)sum];
                        p2 *= 2;              /* next power of two */
                    }
                }
            }
        }
        max_n = n;
    }

    return (n <= max_n) ? cnt[(idx_t)n] : 0;
}
