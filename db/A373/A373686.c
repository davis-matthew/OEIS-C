/*
 * A373686  a(n) is the number of ways n can be written as a sum of a
 *          practical number and two squares.
 * https://oeis.org/A373686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = p + x² + y² where p is a
 *            practical number and 0 ≤ y ≤ x
 * Range:     u8 n <= 1884, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_practical_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_practical_uint32.h"

static idx_t cache_limit = 0;
static uint32_t *spf = NULL;          /* smallest prime factor */
static bool *practical = NULL;        /* flag whether a number is practical */
static idx_t *practical_list = NULL;  /* sorted list of practical numbers */
static size_t practical_count = 0;
static idx_t *squares = NULL;         /* pre‑computed squares up to sqrt(cache_limit) */
static size_t squares_len = 0;

/* Integer square root using a simple linear scan – fast enough for n ≤ 10000. */
static idx_t isqrt_idx(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v)
        r++;
    return r;
}

/* Ensure all helper tables are built up to limit. */
static void ensure_cache(idx_t limit)
{
    if (limit <= cache_limit) return;

    /* Resize spf array */
    free(spf);
    spf = (uint32_t *)malloc(sizeof(uint32_t) * (limit + 1));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {          /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* Resize practical flag array */
    free(practical);
    practical = (bool *)malloc(sizeof(bool) * (limit + 1));
    practical[0] = false;
    practical[1] = true;

    /* Build practical list */
    practical_count = 0;
    free(practical_list);
    practical_list = (idx_t *)malloc(sizeof(idx_t) * (limit + 1));
    for (idx_t m = 1; m <= limit; ++m) {
        bool ok = is_practical_uint32((uint32_t)m, spf);
        practical[m] = ok;
        if (ok) practical_list[practical_count++] = m;
    }

    /* Precompute squares */
    free(squares);
    squares_len = isqrt_idx(limit) + 1;
    squares = (idx_t *)malloc(sizeof(idx_t) * squares_len);
    for (idx_t i = 0; i < squares_len; ++i)
        squares[i] = i * i;

    cache_limit = limit;
}

/* Count unordered pairs (x, y) of non‑negative integers such that
   x² + y² = s. */
static seq_t count_square_pairs(idx_t s)
{
    seq_t cnt = 0;
    for (idx_t x = 0; x < squares_len && squares[x] <= s; ++x) {
        idx_t rem = s - squares[x];
        /* binary search for rem in squares[0..x] */
        idx_t lo = 0, hi = x;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            idx_t val = squares[mid];
            if (val == rem) { cnt++; break; }
            else if (val < rem) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of offset; undefined but safe */
    ensure_cache(n);

    seq_t total = 0;
    for (size_t i = 0; i < practical_count && practical_list[i] <= n; ++i) {
        idx_t rem = n - practical_list[i];
        total += count_square_pairs(rem);
    }
    return total;
}
