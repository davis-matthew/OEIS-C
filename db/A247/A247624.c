/*
 * A247624  Knife numbers (first version): n=concat(a,b) is divisible by a+b;
 *          a,b>0.
 * https://oeis.org/A247624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Knife numbers
 * Definition:
 *   n = concat(a, b) is divisible by a + b
 *   a, b > 0 and b has no leading zeros
 * Sequence is enumerated in increasing order.
 * This implementation builds the sequence lazily using a cache.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Cache storage */
static seq_t *cache = NULL;          /* array of found knife numbers */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t cache_cnt = 0;          /* number of elements stored */

/* Test whether a number is a knife number */
static bool
is_knife_number(seq_t num)
{
    int len = digit_len(num);                /* total number of decimal digits */
    for (int bdigits = 1; bdigits < len; ++bdigits) {
        seq_t powb = pow10((idx_t)bdigits);  /* 10^bdigits   */
        seq_t b    = num % powb;             /* suffix (b)   */
        if (b == 0) continue;               /* b must be > 0*/
        /* reject suffixes that would have leading zeros */
        if (bdigits > 1 && b < powb / 10) continue;
        seq_t a    = num / powb;             /* prefix (a)   */
        seq_t sum  = a + b;
        if (num % sum == 0) return true;    /* condition satisfied */
    }
    return false;
}

/* Ensure that at least min_cap cache slots are available */
static void
ensure_cache(size_t min_cap)
{
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th knife number (1‑based). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence is defined for n ≥ 1 */
    ensure_cache((size_t)n);

    /* Fill the cache up to the requested index. */
    while (cache_cnt < n) {
        seq_t cand = (cache_cnt == 0) ? 1 : cache[cache_cnt - 1] + 1;
        while (!is_knife_number(cand)) ++cand;
        cache[cache_cnt++] = cand;
    }
    return cache[n - 1];
}
