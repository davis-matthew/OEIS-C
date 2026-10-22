/*
 * A303713  Number of n X n 0..1 arrays with every element unequal to 0, 1 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A303713 by recurrence (valid for n > 18) a(n) = 3*a(n‑1) +
 *            2*a(n‑2) +28*a(n‑3) -2*a(n‑4) -42*a(n‑5) -56*a(n‑6) +50*a(n‑7)
 *            -44*a(n‑8) +30*a(n‑9) -44*a(n‑10) +2*a(n‑11) +2*a(n‑12) -a(n‑14)
 *            +a(n‑15). Initial values:
 *            a(1)=1,a(2)=1,a(3)=5,a(4)=17,a(5)=81,a(6)=343,a(7)=1592,a(8)=7551,
 *            a(9)=34546,a(10)=...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Initial 18 terms stored as 64‑bit integers; conversion to seq_t
   occurs on assignment.  */
static const uint64_t init_vals[18] = {
    1ULL, 1ULL, 5ULL, 17ULL, 81ULL, 343ULL, 1592ULL,
    7551ULL, 34546ULL, 158402ULL, 734321ULL, 3389249ULL,
    15610239ULL, 72045856ULL, 332515175ULL, 1533661362ULL,
    7075164898ULL, 32644176977ULL
};

static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static idx_t cache_len = 0;      /* highest index currently cached (0 if none) */

/* Ensure that cache contains values up to at least n. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate space for indices 1 … 18.  */
        cache = (seq_t *)malloc(19 * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);
        for (idx_t i = 1; i <= 18; ++i)
            cache[i] = (seq_t)init_vals[i - 1];
        cache_len = 18;
    }
    if (n <= cache_len) return;

    /* Grow cache to size n+1. */
    size_t new_cap = (size_t)n + 1;          /* indices 1 … n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache = tmp;

    /* Compute every missing term using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 18) {
            cache[i] = (seq_t)init_vals[i - 1];
            continue;
        }
        /* Positive contributions. */
        seq_t pos = 0;
        pos += cache[i - 1] * (seq_t)3;
        pos += cache[i - 2] * (seq_t)2;
        pos += cache[i - 3] * (seq_t)28;
        pos += cache[i - 7] * (seq_t)50;
        pos += cache[i - 9] * (seq_t)30;
        pos += cache[i -11] * (seq_t)2;
        pos += cache[i -12] * (seq_t)2;
        pos += cache[i -15] * (seq_t)1;

        /* Negative contributions. */
        seq_t neg = 0;
        neg += cache[i - 4] * (seq_t)2;
        neg += cache[i - 5] * (seq_t)42;
        neg += cache[i - 6] * (seq_t)56;
        neg += cache[i - 8] * (seq_t)44;
        neg += cache[i -10] * (seq_t)44;
        neg += cache[i -14] * (seq_t)1;

        cache[i] = pos - neg;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* defined for convenience */
    ensure_cache(n);
    return cache[n];
}
