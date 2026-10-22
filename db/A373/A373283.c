/*
 * A373283  Expansion of Sum_{k>=0} x^(7^k) / (1 - 7*x^(7^k)).
 * https://oeis.org/A373283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^(n-1) + (n mod 7 == 0 ? a(n/7) : 0) (Recurrence given
 *            in the OEIS entry) Computes a(n) for n >= 1, with memoisation so
 *            that repeated calls are fast
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 7^i cache ; pow_cache[i] = 7^i for 0 <= i < pow_cache_size   */
static seq_t *pow_cache   = NULL;
static idx_t  pow_cache_size = 0;

/* a(n) cache ; 1‑indexed, a_cache[0] unused                          */
static seq_t *a_cache   = NULL;
static idx_t  a_cache_size = 0;
static idx_t  a_computed_up_to = 0;   /* highest n that has been cached */

/* Ensure pow_cache can hold entries up to index 'need' (need >= 0)   */
static void ensure_pow_cache(idx_t need)
{
    if (need + 1 <= pow_cache_size) return;

    idx_t new_size = need + 1;
    seq_t *new_cache = (seq_t *)realloc(pow_cache,
                                        new_size * sizeof(seq_t));
    if (!new_cache) exit(1);
    pow_cache = new_cache;

    /* initialise when the cache was previously empty            */
    if (pow_cache_size == 0) {
        pow_cache[0] = (seq_t)1;
        pow_cache_size = 1;
    }

    for (idx_t i = pow_cache_size; i < new_size; ++i)
        pow_cache[i] = pow_cache[i - 1] * (seq_t)7;

    pow_cache_size = new_size;
}

/* Ensure a_cache can hold entries up to index 'n' (1‑indexed)       */
static void ensure_a_cache(idx_t n)
{
    if (n + 1 <= a_cache_size) return;

    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache,
                                        new_size * sizeof(seq_t));
    if (!new_cache) exit(1);
    a_cache = new_cache;

    /* initialise new entries to the sentinel value 0              */
    for (idx_t i = a_cache_size; i < new_size; ++i)
        a_cache[i] = (seq_t)0;

    a_cache_size = new_size;
}

/* Public interface: compute a(n) for n >= 1                         */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* caller must not use n=0 */

    /* enlarge caches if necessary                                */
    ensure_a_cache(n);
    ensure_pow_cache(n - 1);

    /* if already computed, return it                              */
    if (n <= a_computed_up_to) return a_cache[n];

    /* compute all missing values up to n in increasing order      */
    for (idx_t i = a_computed_up_to + 1; i <= n; ++i) {
        seq_t val = pow_cache[i - 1];          /* 7^(i-1)            */
        if (i % (idx_t)7 == 0)                 /* add a(i/7) if divisible by 7 */
            val += a_cache[i / (idx_t)7];
        a_cache[i] = val;
    }
    a_computed_up_to = n;
    return a_cache[n];
}
