/*
 * A261481  Number of set partitions of [n] into exactly five parts such that
 *          no part contains two elements with a circular distance less than
 *          three.
 * https://oeis.org/A261481
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A261481: number of set partitions of [n] into exactly five parts
   such that no part contains two elements with a circular distance less than three.
   Recurrence (for n ≥ 15):
       a(n) = 2·a(n‑1) + a(n‑2) + 12·a(n‑3) - 6·a(n‑4) - 18·a(n‑5)
              - 47·a(n‑6) - 32·a(n‑7) + 17·a(n‑8) + 36·a(n‑9) + 36·a(n‑10)
   Initial values:
       a(5)=1, a(6)=3, a(7)=14, a(8)=44, a(9)=138, a(10)=458,
       a(11)=1397, a(12)=4219, a(13)=12974, a(14)=39270
*/

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically allocated array */
static size_t cache_len = 0;         /* number of elements stored (≥ current cached size) */
static size_t computed_to = 4;       /* highest index that has been fully computed */

/* Make sure the cache can hold indices up to at least n. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_len)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!new_cache) abort();          /* allocation failure */
    /* Initialise new entries to 0. */
    for (size_t i = cache_len; i < required; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_len = required;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;          /* undefined for n < 5 */

    ensure_cache(n);

    /* If we already have computed up to n, just return. */
    if (computed_to >= (size_t)n)
        return cache[n];

    /* Initial values for 5 … 14. */
    static const seq_t base[15] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)3, (seq_t)14, (seq_t)44, (seq_t)138,
        (seq_t)458, (seq_t)1397, (seq_t)4219, (seq_t)12974,
        (seq_t)39270
    };

    /* Compute values from max(computed_to+1,5) up to n. */
    for (size_t i = (computed_to >= 4 ? computed_to + 1 : 5); i <= (size_t)n; ++i) {
        if (i <= 14) {
            cache[i] = base[i];
        } else {
            seq_t p = (seq_t)2 * cache[i-1]
                    + cache[i-2]
                    + (seq_t)12 * cache[i-3]
                    + (seq_t)17 * cache[i-8]
                    + (seq_t)36 * cache[i-9]
                    + (seq_t)36 * cache[i-10];
            seq_t q = (seq_t)6 * cache[i-4]
                    + (seq_t)18 * cache[i-5]
                    + (seq_t)47 * cache[i-6]
                    + (seq_t)32 * cache[i-7];
            cache[i] = p - q;  /* P ≥ Q for all n ≥ 15 */
        }
    }
    computed_to = (size_t)n;
    return cache[n];
}
