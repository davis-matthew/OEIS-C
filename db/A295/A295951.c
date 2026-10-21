/*
 * A295951  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n), where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4, b(2) = 5, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   contains_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-2) + b(n)
 * with a(0)=2, a(1)=3,  b(0)=1, b(1)=4
 * and a,b are increasing complementary sequences.
 * The function returns a(n) for any n ≥ 0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_a.h"

/* static storage for the two sequences and a candidate counter */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;
static seq_t next_candidate = 0;   /* next integer to try for b(k) */

/* guarantee capacity for at least need elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    seq_t *tmp_b = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
    a_cache = tmp_a;
    b_cache = tmp_b;
    cache_cap = new_cap;
}

/* initialise static data once */
static void init_once(void)
{
    ensure_capacity(8);          /* small initial capacity */
    a_cache[0] = (seq_t)2;
    a_cache[1] = (seq_t)3;
    b_cache[0] = (seq_t)1;
    b_cache[1] = (seq_t)4;
    cache_size = 2;
    next_candidate = (seq_t)5;   /* b(1)+1 */
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init_once(); initialized = true; }

    /* grow the cache up to n */
    if ((size_t)n >= cache_size) {
        ensure_capacity((size_t)n + 1);

        for (size_t k = cache_size; k <= (size_t)n; ++k) {
            /* find b(k) : least positive int not used in a[0..k-1] */
            seq_t cand = next_candidate;
            while (contains_a(a_cache, k, cand))
                ++cand;

            b_cache[k] = cand;
            next_candidate = cand + 1;   /* start after this for next round */

            /* compute a(k) = a(k-1) + a(k-2) + b(k)   (k ≥ 2) */
            seq_t sum = a_cache[k-1] + a_cache[k-2] + cand;
            a_cache[k] = sum;
        }

        cache_size = (size_t)n + 1;
    }

    return a_cache[(size_t)n];
}
