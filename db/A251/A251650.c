/*
 * A251650  Number of (n+2)X(6+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 6*a(n-1) - 3*a(n-2) + 44*a(n-3) -
 *            390*a(n-4) + 222*a(n-5) + 1617*a(n-6) - 1224*a(n-7) -
 *            1223*a(n-8) + 1020*a(n-9) - 68*a(n-11) for n > 15 Initial
 *            values: a(1)=4140, a(2)=22904, a(3)=74194, a(4)=245842,
 *            a(5)=1563586, a(6)=5167050, a(7)=17185778, a(8)=108553714,
 *            a(9)=360...
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *values = NULL; /* 1‑based array of computed terms */
    static idx_t max_computed = 0; /* largest n with values[n] computed */

    /* Base terms for n = 1 .. 15 */
    /* Index 0 is unused to keep 1‑based indexing */
    static const seq_t base[16] = {
        0ULL,
        4140ULL,
        22904ULL,
        74194ULL,
        245842ULL,
        1563586ULL,
        5167050ULL,
        17185778ULL,
        108553714ULL,
        360921738ULL,
        1210167730ULL,
        7559111602ULL,
        25300264746ULL,
        85512861490ULL,
        527702965490ULL,
        1778470746634ULL
    };

    /* Ensure the cache holds at least up to the base terms */
    if (max_computed < 15) {
        idx_t new_size = (15 > max_computed ? 15 : max_computed) + 1;
        seq_t *tmp = (seq_t *)realloc(values, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, return 0 */
        values = tmp;
        /* Fill base values */
        for (idx_t i = 1; i <= 15; ++i) {
            values[i] = base[i];
        }
        max_computed = 15;
    }

    /* If the requested n is already computed, return it */
    if (n <= max_computed) {
        return values[n];
    }

    /* Extend the cache to accommodate up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(values, new_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    values = tmp;

    /* Coefficients for the recurrence (positive and negative parts) */
    const int pos_idx[5]   = {1, 3, 5, 6, 9};
    const int pos_coeff[5] = {6, 44, 222, 1617, 1020};
    const int neg_idx[5]   = {2, 4, 7, 8, 11};
    const int neg_coeff[5] = {3, 390, 1224, 1223, 68};

    /* Compute terms from max_computed + 1 up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        for (int k = 0; k < 5; ++k) {
            /* Positive contribution */
            sum_pos += values[i - pos_idx[k]] * (seq_t)pos_coeff[k];
            /* Negative contribution */
            sum_neg += values[i - neg_idx[k]] * (seq_t)neg_coeff[k];
        }

        /* The recurrence yields a non‑negative result; compute it safely */
        values[i] = sum_pos - sum_neg;
    }

    max_computed = n;
    return values[n];
}
