/*
 * A399001  The binomial matrix of the Fibonacci numbers, square array read by
 *          upward antidiagonals.
 * https://oeis.org/A399001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are the coordinates on the upward
 *            antidiagonals of the binomial matrix of Fibonacci numbers. For a
 *            given n find the diagonal t such that t*(t+1)/2 <= n <
 *            (t+1)*(t+2)/2, let s = n - t*(t+1)/2, then i = t - s, j = s.
 *            Then A(i,j) = Σ_{m=0}^{|i-j|} binomial(|i-j|,m) * sgn^(|...
 * Range:     i8 n <= 20, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>      /* for sqrtl */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2. */
    idx_t low = 0;
    idx_t high = (idx_t)(sqrtl((long double)2 * (long double)n) + 2);
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 prod = (__int128)mid * (mid + 1);
        idx_t start_mid = (idx_t)(prod / 2);  /* mid*(mid+1)/2 */
        if (start_mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t t = low;

    /* Coordinates on diagonal t */
    __int128 prod = (__int128)t * (t + 1);
    idx_t start_t = (idx_t)(prod / 2);        /* t*(t+1)/2 */
    idx_t offset = n - start_t;              /* 0 <= offset <= t */
    idx_t j = offset;
    idx_t i = t - offset;

    /* Compute d, p, and the sign factor s */
    idx_t d = (i > j) ? i - j : j - i;
    idx_t p = (i < j) ? i : j;
    int s = (j <= i) ? 1 : -1;  /* integer sign */

    /* Precompute Fibonacci values up to index t */
    seq_t *fibs = (seq_t*)malloc((t + 1) * sizeof(seq_t));
    if (!fibs) return 0;  /* allocation failure, returns 0 as a fallback */
    fibs[0] = 0;
    if (t >= 1) fibs[1] = 1;
    for (idx_t k = 2; k <= t; ++k)
        fibs[k] = fibs[k - 1] + fibs[k - 2];

    /* Sum the formula */
    seq_t result = 0;
    for (idx_t m = 0; m <= d; ++m) {
        seq_t bin = binomial(d, m);
        if (s == -1 && ((d - m) & 1))
            bin = -bin;
        seq_t term = bin * fibs[p + m];
        result += term;
    }

    free(fibs);
    return result;
}
