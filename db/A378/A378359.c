/*
 * A378359  a(1) = 0, a(n) = Sum_{digits d in a(n-1)} c(d,n-1), where c(d,k)
 *          is the number of digits d in a(1..k).
 * https://oeis.org/A378359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 315, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=0; for n>1, a(n)=sum_{digits d in a(n‑1)} c(d,n‑1),
   where c(d,k) is the number of digits d that have appeared in a(1)…a(k). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

#ifdef __cplusplus
#include <new>
#endif

/* static state ------------------------------------------------------------- */
static seq_t *seq_cache   = NULL;   /* 1‑based array of computed terms   */
static size_t cache_cap  = 0;       /* allocated capacity (positive)    */
static size_t cache_len  = 0;       /* largest index computed so far    */
static seq_t  digit_cnt[10] = {0};  /* running counts of each decimal digit */

/* ensure the cache array has room for indices up to 'need' */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t old_cap = cache_cap;
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * (new_cap + 1));
        if (!tmp) abort();          /* allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
#ifdef __cplusplus
        /* construct the newly allocated elements (C++ non‑POD types may require construction) */
        for (size_t i = old_cap + 1; i <= new_cap; ++i) {
            new(&seq_cache[i]) seq_t();
        }
#endif
    }
}

/* update global digit counters with all digits of value 'val' */
static void add_value_to_counts(seq_t val)
{
    uint8_t buf[128];
    size_t len = 0;
    get_digits(val, buf, &len);
    for (size_t i = 0; i < len; ++i) {
        unsigned d = buf[i];
        digit_cnt[d] = digit_cnt[d] + (seq_t)1;
    }
}

/* compute a(index) assuming a(index-1) is already in seq_cache */
static void compute_at(size_t index)
{
    /* a(index) = sum over distinct digits of a(index-1) of digit_cnt[dig] */
    uint8_t buf[128];
    size_t len = 0;
    get_digits(seq_cache[index - 1], buf, &len);
    seq_t sum = (seq_t)0;
    bool seen[10] = {0};
    for (size_t i = 0; i < len; ++i) {
        unsigned d = buf[i];
        if (!seen[d]) {
            seen[d] = true;
            sum = sum + digit_cnt[d];
        }
    }
    seq_cache[index] = sum;
    /* record digits of the new term */
    add_value_to_counts(sum);
}

/* initialize cache for a(1) if it has not yet been done */
static void init_cache(void)
{
    if (cache_cap == 0) {
        cache_cap  = 1;
        cache_len  = 1;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1));
        if (!seq_cache) abort();
#ifdef __cplusplus
        new(&seq_cache[1]) seq_t();   /* construct element 1 */
#endif
        seq_cache[1] = (seq_t)0;
        for (size_t i = 0; i < 10; ++i) digit_cnt[i] = (seq_t)0;
        digit_cnt[0] = (seq_t)1;           /* the digit '0' appears once in a(1)=0 */
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n<1, but return 0 */

    size_t ix = (size_t)n;
    init_cache();
    ensure_capacity(ix);

    /* compute terms up to 'ix' if not yet done */
    while (cache_len < ix) {
        ++cache_len;
        compute_at(cache_len);
    }

    return seq_cache[ix];
}
