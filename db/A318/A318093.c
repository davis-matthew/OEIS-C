/*
 * A318093  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3 or
 *          5 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   empirical recurrence for a(n) (n > 21) a(n) = 14*a(n-2)
 *            +18*a(n-3) -23*a(n-4) -69*a(n-5) -69*a(n-6) -38*a(n-7)
 *            +82*a(n-8) +261*a(n-9) +101*a(n-10) -10*a(n-11) -20*a(n-12)
 *            -112*a(n-13) -112*a(n-14) -90*a(n-15) -24*a(n-16) -12*a(n-17)
 *            +4*a(n-18) +4*a(n-19) Initial values (a(1) … a(23)) are taken
 *            fr...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1) … a(23).  a[0] is unused. */
    static const seq_t init[24] = {
        0ULL,              /* dummy for index 0 */
        1ULL, 16ULL, 45ULL, 220ULL, 795ULL,
        3310ULL, 12774ULL, 51298ULL, 201637ULL, 802119ULL,
        3169137ULL, 12571062ULL, 49748730ULL, 197152806ULL,
        780649100ULL, 3092658411ULL, 12248195877ULL,
        48517164179ULL, 192162096401ULL, 761151962411ULL,
        3014780751286ULL, 11941310147017ULL, 47297795017905ULL
    };

    if (n <= 23)
        return init[n];

    /* Allocate array of length n+1 (index 1 … n).  arr[0] remains unused. */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return 0;        /* Fallback on allocation failure. */

    /* Copy known initial values. */
    for (idx_t i = 1; i <= 23; ++i)
        arr[i] = init[i];

    /* Compute via the empirical recurrence. */
    for (idx_t i = 24; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive contributions */
        pos += (seq_t)14 * arr[i - 2];
        pos += (seq_t)18 * arr[i - 3];
        pos += (seq_t)82 * arr[i - 8];
        pos += (seq_t)261 * arr[i - 9];
        pos += (seq_t)101 * arr[i - 10];
        pos += (seq_t)4 * arr[i - 18];
        pos += (seq_t)4 * arr[i - 19];

        /* Negative contributions */
        neg += (seq_t)23 * arr[i - 4];
        neg += (seq_t)69 * arr[i - 5];
        neg += (seq_t)69 * arr[i - 6];
        neg += (seq_t)38 * arr[i - 7];
        neg += (seq_t)10 * arr[i - 11];
        neg += (seq_t)20 * arr[i - 12];
        neg += (seq_t)112 * arr[i - 13];
        neg += (seq_t)112 * arr[i - 14];
        neg += (seq_t)90 * arr[i - 15];
        neg += (seq_t)24 * arr[i - 16];
        neg += (seq_t)12 * arr[i - 17];

        arr[i] = pos - neg;   /* pos > neg for n > 21, so no underflow. */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
