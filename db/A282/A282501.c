/*
 * A282501  Numerator of Sum_{i = 1..n} (if(isprime(i), 0, 1/i)).
 * https://oeis.org/A282501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of \sum_{i=1..n} [!isprime(i)]/i (i.e. add
 *            0 for primes, otherwise 1/i). So a(1)=1, a(2)=1, a(3)=1, a(4)=5,
 *            a(5)=5,
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 34, u64 n <= 81, u128 n <= 164,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

static seq_t *cache_num = NULL;   /* 1‑based cache, cache_num[0] unused */
static size_t cache_cap = 0;      /* current capacity (including dummy index 0) */
static size_t cache_len = 0;      /* highest index computed and stored */

static seq_t cur_num = 0;         /* numerator of current sum */
static seq_t cur_den = 1;         /* denominator of current sum */

/* Ensure the cache has room for at least indices 0..max_i (inclusive).
   Reallocate only when needed. */
static void
ensure_cache(idx_t max_i)
{
    if (cache_cap == 0) /* first time: allocate dummy element 0 */
    {
        cache_cap = 1;
        cache_num = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache_num) abort();
    }
    if ((size_t)max_i + 1 <= cache_cap) return;
    size_t new_cap = (size_t)max_i + 1;
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache_num, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* zero newly allocated part (no effect on numerators) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;
    cache_num = tmp;
    cache_cap = new_cap;
}

/* Main function that returns the numerator a(n).  Handles arbitrary
   calling order of n. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined; caller never uses n==0 */

    /* Fast path: already computed up to n. */
    if (cache_len > 0 && (size_t)n <= cache_len)
        return cache_num[(size_t)n];

    /* Extend cache to accommodate n. */
    ensure_cache(n);

    /* Compute from the next unchecked index up to n. */
    size_t start = (cache_len == 0) ? 1 : cache_len + 1;
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (!is_prime((idx_t)i)) {
            seq_t i_seq = (seq_t)i;
            /* New fraction: cur_num/cur_den + 1/i = (cur_num + cur_den/i)/cur_den */
            seq_t new_num = cur_num * i_seq + cur_den;
            seq_t new_den = cur_den * i_seq;
            seq_t g = gcd(new_num, new_den);
            cur_num = new_num / g;
            cur_den = new_den / g;
        }
        cache_num[i] = cur_num;   /* store numerator for index i */
    }
    cache_len = (size_t)n;
    return cur_num;
}
