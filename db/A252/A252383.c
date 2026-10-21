/*
 * A252383  Number of (n+2) X (7+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9) for n > 12, with initial
 *            values: a(1)=7109, a(2)=1928, a(3)=1610, a(4)=1880, a(5)=2310,
 *            a(6)=2769, a(7)=3495, a(8)=4601, a(9)=5840, a(10)=7723,
 *            a(11)=10599, a(12)=13880
 * Range:     u8 none, u16 n <= 16, u32 n <= 51, u64 n <= 120, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const seq_t init_vals[12] = {
    (seq_t)7109, (seq_t)1928, (seq_t)1610, (seq_t)1880, (seq_t)2310,
    (seq_t)2769, (seq_t)3495, (seq_t)4601, (seq_t)5840, (seq_t)7723,
    (seq_t)10599, (seq_t)13880
};

#define MAX_CACHE 20000   /* sufficient for n up to about 10000 */

static seq_t cache[MAX_CACHE];
static size_t computed_max = 0;

/* Returns the n-th term of OEIS A252383 (a(1) = first term). */
seq_t a(idx_t n)
{
    /* Initialize cache with known terms on first call. */
    if (computed_max == 0) {
        for (size_t i = 0; i < 12 && i < MAX_CACHE; ++i)
            cache[i] = init_vals[i];
        computed_max = 12;
    }

    /* If requested term already computed, return it. */
    if (n <= computed_max)
        return cache[n - 1];

    /* Compute terms up to n using the recurrence. */
    for (size_t i = computed_max + 1; i <= (size_t)n; ++i) {
        if (i <= 12) {
            cache[i - 1] = init_vals[i - 1];
        } else {
            /* a(i) = 4*a(i-3) - 4*a(i-6) + a(i-9) */
            /* Indices shifted by -1 for zero‑based array. */
            cache[i - 1] = 4 * cache[i - 4] - 4 * cache[i - 7] + cache[i - 10];
        }
    }
    computed_max = n;
    return cache[n - 1];
}
