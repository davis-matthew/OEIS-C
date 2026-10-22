/*
 * A359213  Numbers k such that rad(k) - 1 is prime.
 * https://oeis.org/A359213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 5824, u32 n <= 7985, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that rad(k) – 1 is prime.
   The function incrementally tests integers k in increasing order,
   stores each qualifying k, and returns the n‑th term. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/rad.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static buffer to hold discovered terms.  The capacity is chosen
       to comfortably cover the typical test ranges (up to 10 000). */
    static seq_t cache[20000];
    static size_t cache_len = 0;   /* number of terms stored */
    static idx_t next_k = 2;       /* next integer to test */

    while (cache_len < (size_t)n) {
        seq_t rad_k = rad(next_k);               /* product of distinct prime factors of next_k */
        seq_t val   = rad_k - (seq_t)1;          /* rad(k) – 1 */
        if (is_prime_big(val)) {
            cache[cache_len++] = next_k;
            /* If the requested index exceeds our pre‑allocated space, abort. */
            if (cache_len == 20000 && (size_t)n > 20000)
                abort();
        }
        ++next_k;
    }

    return cache[n-1];
}
