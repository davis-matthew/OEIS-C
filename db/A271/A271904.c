/*
 * A271904  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 427", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A271904: Partial sums of the number of active cells in Rule 427
 *
 * The generating function has denominator
 *   (1 - x)^4 * (1 + x)^3 * (1 + x^2)^2 * (1 + x + x^2),
 * which expands to
 *   1 + 0·x - 1·x² - 1·x³ - 2·x⁴ + 1·x⁵ + 2·x⁶ + 2·x⁷ + 1·x⁸
 *   - 2·x⁹ - 1·x¹⁰ - 1·x¹¹ + 0·x¹² + 1·x¹³ .
 *
 * This yields a homogeneous linear recurrence of order 13 for n ≥ 13:
 *
 *   a(n) = a(n‑2) + a(n‑3) + 2·a(n‑4)
 *        - a(n‑5) - 2·a(n‑6) - 2·a(n‑7) - a(n‑8)
 *        + 2·a(n‑9) + a(n‑10) + a(n‑11) - a(n‑13)
 *
 * Initial values (n = 0 … 12):
 *   1, 6, 19, 51, 88, 177, 257, 402, 515, 807, 976, 1333, 1577
 *
 * The function below memoises computed terms for any order of
 * invocations and works with the flexible integer type seq_t.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    (seq_t)1,   (seq_t)6,   (seq_t)19,  (seq_t)51,
    (seq_t)88,  (seq_t)177, (seq_t)257, (seq_t)402,
    (seq_t)515, (seq_t)807, (seq_t)976, (seq_t)1333,
    (seq_t)1577
};

static seq_t *cached = NULL;
static idx_t cache_len = 0;      /* Number of elements allocated (size) */
static idx_t last_computed = (idx_t)-1; /* Highest n that has been filled */

/* Compute a(n) using the recurrence and memoise results */
seq_t a(idx_t n)
{
    /* Ensure initial caching of the first 13 terms */
    if (cache_len == 0) {
        cache_len = 13;
        cached = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cached) return (seq_t)0; /* Out of memory – unlikely */
        for (idx_t i = 0; i < 13; ++i)
            cached[i] = init[i];
        last_computed = 12;
    }

    /* If the requested value is already computed, return it immediately */
    if (n <= last_computed)
        return cached[n];

    /* Grow the cache to accommodate up to n */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cached, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                /* Allocation failed */
        cached = tmp;
        cache_len = new_len;
    }

    /* Compute terms from last_computed + 1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Compute positive and negative sums separately to avoid
           wrap‑around on unsigned types.  The recurrence guarantees
           the positive sum is always ≥ the negative sum. */
        seq_t pos = cached[i - 2] + cached[i - 3];
        pos += cached[i - 4]; pos += cached[i - 4];
        pos += cached[i - 9]; pos += cached[i - 9];
        pos += cached[i - 10];
        pos += cached[i - 11];

        seq_t neg = cached[i - 5];
        neg += cached[i - 6]; neg += cached[i - 6];
        neg += cached[i - 7]; neg += cached[i - 7];
        neg += cached[i - 8];
        neg += cached[i - 13];

        cached[i] = pos - neg;
    }
    last_computed = n;
    return cached[n];
}
