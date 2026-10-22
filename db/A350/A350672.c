/*
 * A350672  a(n) is the smallest number larger than a(n-1) that has only two
 *          digits in common with a(n-1).
 * https://oeis.org/A350672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest integer > a(n‑1) such that * the sum of the
 *            products of the frequencies of each decimal * digit in a(n‑1)
 *            and a(n) equals 2
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* 1‑indexed cache of terms   */
static size_t cache_cap = 0;     /* capacity of cache (index 0 unused)   */
static idx_t cache_size = 0;     /* highest index computed (>=1) */

/* Resize the cache if a larger size is required. */
static void ensure_cache(idx_t required)
{
    if ((size_t)required <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < (size_t)required) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute the decimal digit frequencies of x.  The result is written
 * into the array freq[10]; each element is an unsigned char.  */
static void digit_frequencies(seq_t x, unsigned char freq[10])
{
    for (int i = 0; i < 10; ++i) freq[i] = 0;
    if (x == 0) { freq[0] = 1; return; }
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        freq[d] += 1;
        x /= 10;
    }
}

/* Return the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but harmless */
    if (cache_size == 0) {                /* initialise cache */
        ensure_cache(1);
        cache[1] = (seq_t)1;
        cache_size = 1;
    }
    /* Compute terms up to n on demand. */
    while (cache_size < n) {
        seq_t prev = cache[cache_size];
        unsigned char prev_cnt[10];
        digit_frequencies(prev, prev_cnt);

        seq_t cand = prev + (seq_t)1;
        while (1) {
            unsigned char cand_cnt[10];
            digit_frequencies(cand, cand_cnt);
            int common = 0;
            for (int d = 0; d < 10; ++d)
                common += (int)prev_cnt[d] * (int)cand_cnt[d];
            if (common == 2) break;
            ++cand;
        }

        ensure_cache(cache_size + 1);
        cache[++cache_size] = cand;
    }
    return cache[n];
}
