/*
 * A268620  Numbers whose digital sum is a multiple of 4.
 * https://oeis.org/A268620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth non‑negative integer x with digit_sum(x) % 4 == 0,
 *            (a(1) = 0). The digit_sum function is supplied by the framework
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Cached sequence of terms. */
static seq_t *cache       = NULL;   /* array of found terms                */
static size_t cache_cap   = 0;      /* allocated capacity                 */
static size_t cache_size  = 0;      /* number of terms currently stored    */
static seq_t next_value   = 0;      /* next integer to test after a(1)     */

/* Ensure the cache can hold at least `need` elements.  */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < need)
        new_cap = need;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* unrecoverable allocation failure */
    cache   = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term (n >= 1).  Lower indices are undefined but
   return 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* below offset – defined arbitrarily */

    /* Initialise cache on first call. */
    if (!cache) {
        ensure_cache_capacity(1);
        cache[0] = 0;          /* a(1) = 0 */
        cache_size = 1;
        next_value = 1;        /* start testing from 1 */
    }

    /* Generate terms until we have at least n. */
    while (cache_size < n) {
        seq_t s = digit_sum(next_value);
        /* Check if the digit sum is a multiple of 4. */
        if ((s % (seq_t)4) == 0) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = next_value;
        }
        ++next_value;
    }

    return cache[n - 1];
}
