/*
 * A251646  Number of (n+2)X(2+2) 0..3 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 0 2 4 5 7 or 9.
 * https://oeis.org/A251646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 4*a(n-1) - 2*a(n-2) + 4*a(n-3) -
 *            36*a(n-4) + 22*a(n-5) + 59*a(n-6) - 40*a(n-7) - 20*a(n-8) +
 *            10*a(n-9) for n > 15 Initial values: a(1) = 448, a(2) = 908,
 *            a(3) = 1928, a(4) = 4008, a(5) = 10248, a(6) = 22904, a(7) =
 *            49956, a(8) = 129296, a(9) = 300656, a(10) = 689192, a...
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t  cache_size = 0;        /* number of terms stored */

/* initial values for n = 1 .. 15 */
static const seq_t init_vals[15] = {
    448ULL, 908ULL, 1928ULL, 4008ULL, 10248ULL, 22904ULL,
    49956ULL, 129296ULL, 300656ULL, 689192ULL, 1792064ULL,
    4317696ULL, 10331264ULL, 26936064ULL, 66797136ULL
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not defined for n < 1, return 0 */

    /* Ensure cache is large enough */
    if (n > cache_size) {
        idx_t new_size = n;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!cache) return 0; /* allocation failure */

        /* fill in from old size up to new_size */
        for (idx_t i = cache_size; i < new_size; ++i) {
            idx_t idx = i + 1; /* 1‑based index */
            seq_t val;
            if (idx <= 15) {
                val = init_vals[idx - 1];
            } else {
                /* recurrence using previous terms */
                /* sum of positive coefficients */
                seq_t sum_pos = 0;
                sum_pos += cache[i - 1] * (seq_t)4; /* 4*a(n-1) */
                sum_pos += cache[i - 3] * (seq_t)4; /* 4*a(n-3) */
                sum_pos += cache[i - 5] * (seq_t)22;/* 22*a(n-5) */
                sum_pos += cache[i - 6] * (seq_t)59;/* 59*a(n-6) */
                sum_pos += cache[i - 9] * (seq_t)10;/* 10*a(n-9) */

                /* subtract contributions of negative coefficients */
                sum_pos -= cache[i - 2] * (seq_t)2;  /* -2*a(n-2) */
                sum_pos -= cache[i - 4] * (seq_t)36; /* -36*a(n-4) */
                sum_pos -= cache[i - 7] * (seq_t)40; /* -40*a(n-7) */
                sum_pos -= cache[i - 8] * (seq_t)20; /* -20*a(n-8) */

                val = sum_pos;
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }
    return cache[n - 1];
}
