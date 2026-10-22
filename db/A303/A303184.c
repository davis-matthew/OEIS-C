/*
 * A303184  Number of 4Xn 0..1 arrays with every element equal to 0, 1, 2, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is given by the empirical linear recurrence of order 22 for
 *            n > 22. The first 22 terms are hard‑coded (a(1)=8 ...
 *            a(22)=19364081749). Coefficients are: a(n) = 4 a(n‑1) –4 a(n‑2)
 *            +4 a(n‑3) –9 a(n‑4) +6 a(n‑5) –5 a(n‑6) +4 a(n‑7) –4 a(n‑8) +14
 *            a(n‑9) –9 a(n‑10) –2 a(n‑12) +8 a(n‑13) +2 a(n‑14)...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Base values for n = 1..22 */
static const seq_t base[23] = {
    (seq_t)0,          /* a(0) unused */
    (seq_t)8,          /* a(1) */
    (seq_t)81,         /* a(2) */
    (seq_t)135,        /* a(3) */
    (seq_t)297,        /* a(4) */
    (seq_t)931,        /* a(5) */
    (seq_t)2569,       /* a(6) */
    (seq_t)6668,       /* a(7) */
    (seq_t)17934,      /* a(8) */
    (seq_t)48674,      /* a(9) */
    (seq_t)131037,     /* a(10) */
    (seq_t)353419,     /* a(11) */
    (seq_t)953231,     /* a(12) */
    (seq_t)2569308,    /* a(13) */
    (seq_t)6928326,    /* a(14) */
    (seq_t)18683550,   /* a(15) */
    (seq_t)50378075,   /* a(16) */
    (seq_t)135840557,  /* a(17) */
    (seq_t)366289949,  /* a(18) */
    (seq_t)987683660,  /* a(19) */
    (seq_t)2663241442, /* a(20) */
    (seq_t)7181313510, /* a(21) */
    (seq_t)19364081749/* a(22) */
};

/* Offsets for the recurrence (no a(n‑11) term) */
static const idx_t offsets[16] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17
};

/* Coefficients corresponding to the offsets above */
static const int coeffs[16] = {
    4, -4, 4, -9, 6, -5, 4, -4, 14, -9, -2, 8, 2, -8, -4, 4
};

seq_t a(idx_t n)
{
    /* Return directly if within the hard‑coded range */
    if (n <= 22)
        return base[n];

    /* Static memoization structure */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated length minus one (indexable 0..cap) */
    static idx_t last_computed = 22;  /* largest n for which cache[n] is valid */
    static bool initialized = false;

    /* First time: allocate and store the base values */
    if (!initialized) {
        size_t initial_cap = 64;
        if (initial_cap < 23)
            initial_cap = 23;
        cache = (seq_t *)malloc((initial_cap + 1) * sizeof(seq_t));
        if (!cache)
            abort();  /* allocation failure */
        for (idx_t i = 1; i <= 22; ++i)
            cache[i] = base[i];
        cache_cap = initial_cap;
        initialized = true;
    }

    /* Grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp)
            abort();  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n if not already done */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 16; ++k) {
                idx_t j = offsets[k];
                int c  = coeffs[k];
                if (c > 0)
                    pos += cache[i - j] * (seq_t)c;
                else
                    neg += cache[i - j] * (seq_t)(-c);
            }
            cache[i] = pos - neg;  /* pos ≥ neg by definition of the recurrence */
        }
        last_computed = n;
    }

    return cache[n];
}
