/*
 * A273357  Numbers n such that the decimal number concat(2,n) is a square.
 * https://oeis.org/A273357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 13085, u64 n <= 13085,
 *            u128 n <= 13085, big any n
 * Validated: standard, pass, 59 terms through n = 9819 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer n such that concat(2,n) is a perfect square. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    static seq_t *cache = NULL;   /* cache of found terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_m = 2;      /* next m to test for a new square */

    if (n == 0) return 0;        /* offset is 1; caller should not request n==0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Square of the current candidate */
        seq_t sq = (seq_t)next_m * (seq_t)next_m;
        int d = num_digits(sq);                    /* number of decimal digits of sq */
        /* d >= 2 because the first digit of any matching square must be 2. */

        /* Leading digit of sq: 2 ? */
        seq_t pow10_d_1 = pow10((idx_t)(d - 1));
        if (sq / pow10_d_1 == (seq_t)2) {
            /* n = sq - 2*10^(d-1) */
            seq_t n_val = sq - 2 * pow10_d_1;
            seq_t lower = pow10((idx_t)(d - 2));   /* 10^(d-2)  (>=1 because d>=2) */

            if (n_val >= lower) {                 /* ensures n has exactly d-1 digits */
                /* Ensure cache capacity */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = n_val;
            }
        }
        next_m++;
    }

    return cache[n - 1];
}
