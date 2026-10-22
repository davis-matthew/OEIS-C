/*
 * A304220  Number of nX7 0..1 arrays with every element unequal to 0, 1, 3 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) +10·a(n-3) +5·a(n-4) –2·a(n-5) –13·a(n‑6)
 *            –16·a(n‑7) –8·a(n‑8) +10·a(n‑9) +22·a(n‑10) +9·a(n‑11)
 *            –20·a(n‑12) –22·a(n‑13) –4·a(n‑14) +14·a(n‑15) +12·a(n‑16)
 *            +6·a(n‑17) –6·a(n‑18) (for n > 24) Initial terms: a(1) = 21,
 *            a(2) = 103, a(3) = 93, …, a(24) = 131237610194
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static const seq_t init_vals[24] = {
    21ULL,   103ULL,   93ULL,    190ULL,    509ULL,
    1628ULL, 4263ULL,  11307ULL, 32315ULL,  89108ULL,
    241808ULL, 671482ULL, 1860876ULL, 5112838ULL, 14112560ULL,
    39021398ULL, 107604674ULL, 296850358ULL, 819720184ULL,
    2262238142ULL, 6242240358ULL, 17229841514ULL,
    47554661392ULL, 131237610194ULL
};

static const int coeff[18] = {
    1,  1, 10, 5, -2, -13, -16, -8,
    10, 22, 9, -20, -22, -4, 14, 12,
    6, -6
};

static seq_t *cache      = NULL;   /* 1‑based cache, cache[0] unused */
static size_t cache_len  = 0;       /* allocated size in cache elements */
static size_t max_filled = 0;       /* last computed index */

static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* include index 0 */
    if (cache_len < needed) {
        size_t newsize = needed > 128 ? needed : 128;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = newsize;
    }
}

static void fill_cache(idx_t n)
{
    if ((size_t)n <= max_filled) return;
    for (size_t i = max_filled + 1; i <= (size_t)n; ++i) {
        if (i <= 24) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 18; ++k) {
                size_t offset = i - (k + 1);
                seq_t val = cache[offset];
                int c = coeff[k];
                if (c >= 0) {
                    pos += mul_small(val, (unsigned int)c);
                } else {
                    neg += mul_small(val, (unsigned int)(-c));
                }
            }
            cache[i] = pos - neg;   /* recurrence guarantees non‑negative result */
        }
    }
    max_filled = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* out of bounds, safe to return 0 */
    ensure_cache_size(n);
    fill_cache(n);
    return cache[n];
}
