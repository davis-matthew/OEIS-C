/*
 * A302619  Number of nX4 0..1 arrays with every element equal to 0, 1, 2 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 6*a(n-3) - a(n-4) - 8*a(n-5) -
 *            7*a(n-6) + 2*a(n-7) + 5*a(n-8) for n > 10 Initial values: a(1) =
 *            8, a(2) = 37, a(3) = 45, a(4) = 130, a(5) = 367, a(6) = 917,
 *            a(7) = 2533, a(8) = 6871, a(9) = 18334, a(10) = 49596
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[10] = {
    8, 37, 45, 130, 367, 917, 2533, 6871, 18334, 49596
};

/* Cache for computed values up to a maximum of 10000.
   a(0) is unused.  a(1) .. a(10) are initialized lazily. */
static seq_t cache[10001] = {0};
static idx_t cached_up_to = 0;

/* Ensure the cache is initialized for the first 10 terms. */
static void init_cache(void)
{
    if (cached_up_to < 10) {
        for (idx_t i = 1; i <= 10; ++i) {
            cache[i] = init[i - 1];
        }
        cached_up_to = 10;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 10) return init[n - 1];

    init_cache();

    if (n > cached_up_to) {
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            seq_t a6 = cache[i - 6];
            seq_t a7 = cache[i - 7];
            seq_t a8 = cache[i - 8];

            seq_t sum = a1 + 3 * a2 + 6 * a3;
            sum -= a4;
            sum -= 8 * a5;
            sum -= 7 * a6;
            sum += 2 * a7;
            sum += 5 * a8;
            cache[i] = sum;
        }
        cached_up_to = n;
    }
    return cache[n];
}
