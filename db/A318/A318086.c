/*
 * A318086  Number of nX3 0..1 arrays with every element unequal to 0, 2, 3 or
 *          5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 35, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318086
 *  Empirical linear recurrence (order 24):
 *      a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) + 4*a(n-4) -12*a(n-5) +7*a(n-6)
 *             -12*a(n-7) +20*a(n-8) -6*a(n-9) +13*a(n-10) +10*a(n-11)
 *             -19*a(n-12) -2*a(n-13) -26*a(n-14) +26*a(n-15) -4*a(n-16)
 *             +10*a(n-17) -10*a(n-18) -2*a(n-19) - a(n-20) + a(n-22) + a(n-24)
 *
 *  Initial terms (n=1..24):
 *      1, 2, 2, 7, 7, 11, 15, 62, 104, 141, 275,
 *      741, 1325, 2194, 4346, 9703, 17959, 32463,
 *      64123, 132534, 250208, 470401, 923631, 1843613
 */

#include <stdlib.h>

static const seq_t init_terms[25] = {
    0ULL,      /* dummy for index 0 */
    1, 2, 2, 7, 7, 11, 15, 62, 104, 141, 275,
    741, 1325, 2194, 4346, 9703, 17959, 32463,
    64123, 132534, 250208, 470401, 923631, 1843613
};

seq_t a(idx_t n)
{
    /*  Use a memoized array to store computed values up to the largest n requested */
    static seq_t *values = NULL;           /* 1‑indexed array of a(k) */
    static size_t cur_n  = 0;              /* maximum n already computed (0 = none) */

    if (n <= 24) {
        return init_terms[n];
    }

    /* Allocate/expand the values array if necessary */
    size_t needed = (size_t)n + 1;        /* indices 0..n inclusive */
    if (!values || needed > cur_n + 1) {
        size_t new_size = needed;
        if (!values) {
            values = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            values = (seq_t *)realloc(values, new_size * sizeof(seq_t));
        }
        if (!values) {
            /* Out of memory – fall back to returning 0 (should not happen in tests) */
            return 0;
        }

        /* Initialize the first 24 terms */
        for (idx_t i = cur_n + 1; i <= 24 && i <= n; ++i) {
            values[i] = init_terms[i];
        }
        cur_n = (idx_t)24;
    }

    /* Compute any remaining terms up to n */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        sum += (seq_t)2 * values[i - 1];
        sum -= values[i - 2];
        sum += (seq_t)2 * values[i - 3];
        sum += (seq_t)4 * values[i - 4];
        sum -= (seq_t)12 * values[i - 5];
        sum += (seq_t)7 * values[i - 6];
        sum -= (seq_t)12 * values[i - 7];
        sum += (seq_t)20 * values[i - 8];
        sum -= (seq_t)6 * values[i - 9];
        sum += (seq_t)13 * values[i - 10];
        sum += (seq_t)10 * values[i - 11];
        sum -= (seq_t)19 * values[i - 12];
        sum -= (seq_t)2 * values[i - 13];
        sum -= (seq_t)26 * values[i - 14];
        sum += (seq_t)26 * values[i - 15];
        sum -= (seq_t)4 * values[i - 16];
        sum += (seq_t)10 * values[i - 17];
        sum -= (seq_t)10 * values[i - 18];
        sum -= (seq_t)2 * values[i - 19];
        sum -= values[i - 20];
        sum += values[i - 22];
        sum += values[i - 24];
        values[i] = sum;
    }
    cur_n = (idx_t)n;

    return values[n];
}
