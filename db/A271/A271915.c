/*
 * A271915  Number of ways to choose three distinct points from a 5 X n grid
 *          so that they form an isosceles triangle.
 * https://oeis.org/A271915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) for n > 18 Initial values:
 *            a(1)=0, a(2)=24, a(3)=108, a(4)=248, a(5)=444, a(6)=672,
 *            a(7)=932, a(8)=1204, a(9)=1512, a(10)=1836, a(11)=2188,
 *            a(12)=2548, a(13)=2936, a(14)=3332, a(15)=3756, a(16)=4192,
 *            a(17)=4656, a(18)=5128
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[18] = {
    0ull, 24ull, 108ull, 248ull, 444ull, 672ull, 932ull, 1204ull,
    1512ull, 1836ull, 2188ull, 2548ull, 2936ull, 3332ull, 3756ull,
    4192ull, 4656ull, 5128ull
};

static seq_t cache[10001];
static size_t computed_up_to = 0;

seq_t a(idx_t n)
{
    if (n <= (idx_t)18)
        return init[(size_t)n - 1];

    size_t N = (size_t)n;

    if (computed_up_to < 18) {
        for (size_t i = 1; i <= 18; ++i)
            cache[i - 1] = init[i - 1];
        computed_up_to = 18;
    }

    if (N <= computed_up_to)
        return cache[N - 1];

    for (size_t i = computed_up_to + 1; i <= N; ++i) {
        cache[i - 1] = 2 * cache[i - 2] - 2 * cache[i - 4] + cache[i - 5];
    }

    computed_up_to = N;
    return cache[N - 1];
}
