/*
 * A295059  Solution of the complementary equation a(n) = 2*a(n-1) + b(n-2),
 *          where a(0) = 1, a(1) = 4, b(0) = 2, b(1) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + b(n-2), a(0)=1, a(1)=4; b(0)=2, b(1)=3; a, b
 *            are increasing complementary sequences. This implementation
 *            lazily computes terms and memoizes them so that calls may come
 *            in any order
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Binary search for a value in a sorted array of seq_t.  Returns 1 if
   found, 0 otherwise. */
static int seq_contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return 1;
    }
    return 0;
}

/* Memoized computation of a(n). */
seq_t a(idx_t n)
{
    /* static caches */
    static seq_t *a_cache = NULL;
    static seq_t *b_cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms computed (>=0) */

    /* Ensure we can address index n. */
    if (cache_len <= (size_t)n) {
        /* Resize if necessary. */
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            if (new_cap < needed) new_cap = needed;
            a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            b_cache = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Generate terms up to n. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                a_cache[0] = (seq_t)1;
                b_cache[0] = (seq_t)2;
            } else if (i == 1) {
                a_cache[1] = (seq_t)4;
                b_cache[1] = (seq_t)3;
            } else {
                /* a(i) = 2*a(i-1) + b(i-2) */
                a_cache[i] = (seq_t)2 * a_cache[i - 1] + b_cache[i - 2];

                /* Find the next b(i): smallest integer > b(i-1) not in a(0..i) */
                seq_t cand = b_cache[i - 1] + (seq_t)1;
                while (seq_contains(a_cache, i + 1, cand)) {
                    ++cand;
                }
                b_cache[i] = cand;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return a_cache[(size_t)n];
}
