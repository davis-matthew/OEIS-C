/*
 * A272405  Numbers n such that sum of the divisors of n is not of the form
 *          x^2 + y^2 + z^2 where x, y, z are integers.
 * https://oeis.org/A272405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 6130, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9960 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A272405: numbers n such that σ(n) is of the form 4^a(8b+7),
 * i.e., σ(n) is not representable as a sum of three squares. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: test whether m is of the form 4^a(8b+7). */
static inline bool sigma_is_forbidden(seq_t m)
{
    while (m % 4 == 0) {
        m /= 4;
    }
    return m % 8 == 7;
}

/* Cached sequence values. */
static seq_t  *cache      = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cap  = 0;
static idx_t  last_checked = 0;

/* Public function returning the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;              /* undefined behaviour for the sequence */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        ++last_checked;
        seq_t s = sigma_of(last_checked);

        if (sigma_is_forbidden(s)) {         /* sigma(last_checked) is of the form 4^a(8b+7) */
            /* Grow cached array if necessary. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = mem_new(seq_t, new_cap);
                if (!tmp) {
                    /* In a real program you would handle allocation failure. */
                    abort();
                }
                for (size_t i = 0; i < cache_len; ++i) {
                    tmp[i] = cache[i];
                }
                mem_free(cache);
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];
}
