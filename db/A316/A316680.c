/*
 * A316680  The integer 1358 and its infinite continuation (when iterating the
 *          rule explained in A316650 and in the Comment section here).
 * https://oeis.org/A316680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1358; for n > 1, let s = digit_sum(a(n‑1)); a(n) =
 *            concat( a(n‑1)/s , a(n‑1)%s ); /* q * 10^k + r, k = digit_len(r)
 * Range:     u8 none, u16 n <= 3, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   digit_sum, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* cache is 1‑indexed: cache[0] == a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    if (size == 0) {                 /* first time, initialize */
        cap   = 64;
        cache = mem_new(seq_t, cap);
        if (!cache) {
            /* perror omitted; return 0 on allocation failure */
            return 0;
        }
        cache[0] = (seq_t)1358;     /* a(1) */
        size = 1;
    }

    /* extend cache until we reach index n */
    while (size < n) {
        seq_t prev = cache[size - 1];

        seq_t s = digit_sum(prev);          /* sum of decimal digits of prev */
        seq_t q = prev / s;
        seq_t r = prev % s;

        int k = num_digits(r);               /* number of decimal digits of r */

        seq_t newv = q * pow10((idx_t)k) + r;

        if (size >= cap) {                  /* need more space */
            cap *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) {
                /* realloc failed, keep old cache to avoid corruption */
                return 0;
            }
            cache = tmp;
        }

        cache[size++] = newv;
    }

    return cache[n - 1];
}
