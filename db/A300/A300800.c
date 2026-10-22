/*
 * A300800  Number of nX4 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 4 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A300800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300800: Number of n×4 0..1 arrays with a 4‑cell
 * sum‑constraint, upper left element zero.
 *
 * Recurrence (empirical, from OEIS entry):
 *   a(n) = 13*a(n-1) + 14*a(n-2) - 44*a(n-3) - 63*a(n-4)
 *          + a(n-5) - 17*a(n-6) + 11*a(n-7) - 36*a(n-8)
 *          - 6*a(n-9) - a(n-10) + a(n-11)
 * Initial terms (a(1)…a(11)):
 *   8, 128, 1672, 23096, 317504, 4369224,
 *   60123512, 827349312, 11385017480, 156667360584,
 *   2155873946064
 * The function is memoised; it works for any unsigned integer type
 * (uint8_t … unsigned __int128) and for arbitrary‑precision classes.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset */

    static seq_t *memo = NULL;         /* cached values */
    static size_t memo_cap = 0;        /* allocated capacity */
    static idx_t  memo_len = 0;        /* highest index computed */

    /* coefficients of the recurrence */
    static const int coeff[11] = {
        13, 14, -44, -63, 1, -17, 11, -36, -6, -1, 1
    };

    /* initial terms, index 0 unused */
    static const seq_t init[12] = {
        0ULL,
        8ULL, 128ULL, 1672ULL, 23096ULL, 317504ULL,
        4369224ULL, 60123512ULL, 827349312ULL, 11385017480ULL,
        156667360584ULL, 2155873946064ULL
    };

    /* --- allocate / initialise cache on first call ------------------- */
    if (memo == NULL) {
        memo_cap = 12; /* need indices 0 … 11 */
        memo = (seq_t*)calloc(memo_cap, sizeof(seq_t));
        /* copy known initial values */
        for (idx_t i = 1; i <= 11; ++i)
            memo[i] = init[i];
        memo_len = 11;
    }

    /* If value already computed, return it */
    if (n <= memo_len) return memo[n];

    /* Grow the cache so that indices up to n are available */
    if ((size_t)n + 1 > memo_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) {            /* allocation failed – abort */
            exit(1);
        }
        /* initialise newly allocated part to zero (realloc may leave garbage) */
        for (size_t i = memo_cap; i < new_cap; ++i)
            tmp[i] = 0;
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = memo_len + 1; i <= n; ++i) {
        seq_t plus  = 0;
        seq_t minus = 0;
        for (int k = 0; k < 11; ++k) {
            seq_t t = memo[i - (k + 1)];
            if (coeff[k] >= 0) {
                plus += (seq_t)coeff[k] * t;
            } else {
                minus += (seq_t)(-coeff[k]) * t;
            }
        }
        memo[i] = plus - minus;     /* modulo arithmetic on unsigned type */
    }
    memo_len = n;
    return memo[n];
}
