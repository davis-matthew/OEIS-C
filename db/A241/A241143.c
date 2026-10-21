/*
 * A241143  Numbers n such that n and 7n share at least one digit.
 * https://oeis.org/A241143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a brute‑force search to find integers n for which n and 7*n share
   at least one decimal digit.  The function caches previously found
   values and works for any n ≥ 1 with O(n) time. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* True if a and b share a decimal digit. */
static bool common_digit(seq_t a, seq_t b)
{
    unsigned mask_a = 0;
    while (a > 0) {
        unsigned d = (unsigned)(a % 10);
        mask_a |= 1u << d;
        a /= 10;
    }
    while (b > 0) {
        unsigned d = (unsigned)(b % 10);
        if (mask_a & (1u << d))
            return true;
        b /= 10;
    }
    return false;
}

/* Cached sequence values.  cache[0] holds a(1), cache[1] holds a(2), … */
static seq_t *cache = NULL;       /* array of cached terms   */
static size_t cache_cap = 0;      /* capacity of cache array */
static idx_t   found_cnt = 0;     /* number of terms already found (a(1)..a(found_cnt)) */
static seq_t  next_candidate = 1; /* integer to test next for the property */

/* Return the n‑th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Offset handling – as the OEIS offset is 1, return 0 if n==0. */
    if (n == 0)
        return 0;

    /* Return cached value if already computed. */
    if (n <= found_cnt)
        return cache[n - 1];

    /* Expand cache if necessary. */
    if (cache_cap < found_cnt + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < found_cnt + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;               /* out of memory – return 0 as a safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the desired index. */
    while (found_cnt < n) {
        if (common_digit(next_candidate, next_candidate * (seq_t)7)) {
            cache[found_cnt++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
