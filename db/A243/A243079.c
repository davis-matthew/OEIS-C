/*
 * A243079  Numbers n such that A = n - DigitProd(n) is divisible by the
 *          largest power of 10 <= A.
 * https://oeis.org/A243079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243079:
   Numbers n such that A = n - DigitProd(n) is divisible by the largest power of 10 ≤ A.
   The digit product must be non‑zero (i.e., n contains no zero digit).  For A = 0 the
   condition is considered satisfied.  The function returns the n‑th term of this
   infinite increasing sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < (size_t)n) {
        /* Determine first candidate to test */
        seq_t candidate;
        if (cache_len == 0) {
            candidate = (seq_t)1;
        } else {
            candidate = cache[cache_len - 1] + (seq_t)1;
        }

        /* Scan forward until a valid term is found */
        for (;;) {
            seq_t dp = digit_product(candidate);

            /* Skip numbers that contain a 0 digit (dp == 0) */
            if (dp == (seq_t)0) {
                candidate = candidate + (seq_t)1;
                continue;
            }

            seq_t A = candidate - dp;

            bool ok;
            if (A == (seq_t)0) {
                ok = true;               /* A = 0 always satisfies the condition */
            } else {
                /* Find the largest power of 10 ≤ A */
                seq_t pow10 = (seq_t)1;
                while (pow10 <= A / (seq_t)10) {
                    pow10 = pow10 * (seq_t)10;
                }
                ok = (A % pow10 == (seq_t)0);
            }

            if (ok) {
                /* Ensure capacity in the cache */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    if (new_cap < cache_len + 1)
                        new_cap = cache_len + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();          /* allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = candidate;
                break;
            }

            candidate = candidate + (seq_t)1;
        }
    }

    return cache[n - 1];
}
