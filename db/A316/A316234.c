/*
 * A316234  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3,
 *          4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
 * a(n) = 3*a(n-1) +10*a(n-3) -19*a(
 *          n-4) -14*a(n-5) -23*a(n-7) +17*a(n-8)
 *          -44*a(n-9) +131*a(n-10) +77*a(n-11)
 *          -105*a(n-12) -165*a(n-13) +99*a(n-14)
 *          +78*a(n-15) -42*a(n-16) -6*a(n-17) +4*a(n-18)
 *   for n > 20
 * Initial values (n=1..20):
 * 3, 9, 14, 29, 71, 153, 358, 956, 2612, 7512,
 * 22712, 70354, 221685, 707752, 2276384,
 * 7352905, 23817277, 77283822, 251019710, 815800421
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Base values a(1) .. a(20) */
    static const seq_t base[20] = {
        3ULL, 9ULL, 14ULL, 29ULL, 71ULL, 153ULL, 358ULL,
        956ULL, 2612ULL, 7512ULL, 22712ULL, 70354ULL,
        221685ULL, 707752ULL, 2276384ULL, 7352905ULL,
        23817277ULL, 77283822ULL, 251019710ULL, 815800421ULL
    };

    if (n <= 20)
        return base[n - 1];

    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)
        return 0;  /* out of memory – caller should not rely on this */

    /* Fill known values */
    for (size_t i = 1; i <= 20; ++i)
        arr[i] = base[i - 1];

    /* Compute terms using the empirical recurrence */
    for (size_t i = 21; i <= (size_t)n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += arr[i - 1] * 3;
        pos += arr[i - 3] * 10;
        pos += arr[i - 8] * 17;
        pos += arr[i -10] * 131;
        pos += arr[i -11] * 77;
        pos += arr[i -14] * 99;
        pos += arr[i -15] * 78;
        pos += arr[i -18] * 4;

        neg += arr[i - 4] * 19;
        neg += arr[i - 5] * 14;
        neg += arr[i - 7] * 23;
        neg += arr[i - 9] * 44;
        neg += arr[i -12] * 105;
        neg += arr[i -13] * 165;
        neg += arr[i -16] * 42;
        neg += arr[i -17] * 6;

        arr[i] = pos - neg;  /* subtraction is safe because result is positive */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
