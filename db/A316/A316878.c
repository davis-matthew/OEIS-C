/*
 * A316878  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n‑1) + 4*a(n‑2)
 *            + 14*a(n‑3) - 19*a(n‑4) - 63*a(n‑5) - 93*a(n‑6) + 71*a(n‑7) +
 *            279*a(n‑8) + 285*a(n‑9) - 86*a(n‑10) - 418*a(n‑11) - 276*a(n‑12)
 *            + 28*a(n‑13) + 90*a(n‑14) - 9*a(n‑15) for n > 17, with initial
 *            values a(1)=4, a(2)=14, a(3)=37, a(4)=105,...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* coefficient array: coeff[k] multiplies a(n‑k‑1) */
static const int coeff[15] = {
    2,   4,  14, -19, -63, -93,  71, 279, 285, -86, -418, -276, 28, 90, -9
};

/* initial 17 terms (0‑based index corresponds to a(index+1)) */
static const seq_t init17[17] = {
    4, 14, 37, 105, 388, 1280, 4121, 13933, 46641, 154593,
    516270, 1725502, 5753200, 19197252, 64095188, 213911810, 713904833
};

/* cache for computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* enlarge cache to hold at least 'cap' elements */
static void ensure_cache_capacity(size_t cap)
{
    if (cap <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < cap)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) for n ≥ 1, with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 to avoid UB */
        return 0;

    /* initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache_capacity(17);
        for (size_t i = 0; i < 17; ++i)
            cache[i] = init17[i];
        cache_len = 17;
    }

    /* return cached value if already computed */
    if ((size_t)(n - 1) < cache_len)
        return cache[n - 1];

    /* extend cache up to n */
    ensure_cache_capacity((size_t)n);
    for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
        seq_t sum = 0;
        for (int i = 0; i < 15; ++i) {
            seq_t term = cache[idx - 1 - i];           /* a(idx – i)   */
            sum += term * (seq_t)coeff[i];
        }
        cache[idx] = sum;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
