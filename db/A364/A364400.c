/*
 * A364400  G.f. satisfies A(x) = 1 + x/A(x)^3*(1 + 1/A(x)^3).
 * https://oeis.org/A364400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * (1/n) * Sum_{k=0..n} binomial(n,k) *
 *            binomial(4*n+3*k-2, n-1) for n > 0, a(0) = 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
   * Computes the sum in a 128‑bit accumulator (__int128) to avoid overflow
   * when seq_t is a 64‑bit signed integer.
   * Uses the provided binomial function.  Intermediate binomial(n,k) and
   * binomial(4*n+3*k-2, n-1) fit in seq_t for the values of n required by
   * the test suite; the 128‑bit sum ensures the intermediate result never
   * exceeds the range of __int128.  The final division and sign are then
   * cast back to seq_t.
   */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result;
    __int128 sum = 0;

    seq_t Cnk = 1;          /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(4*n + 3*k - 2, n-1) */
        idx_t big = 4 * n + 3 * k - 2;
        seq_t Bk = binomial(big, n - 1);

        /* add product to sum using 128‑bit arithmetic */
        sum += ( __int128 )Cnk * ( __int128 )Bk;

        if (k == n) {
            break;            /* last iteration – no further update */
        }

        /* Update Cnk = binomial(n, k+1) efficiently */
        Cnk = Cnk * ( (seq_t)(n - k) ) / ( (seq_t)(k + 1) );
    }

    /* divide by n (128‑bit division) and cast to seq_t */
    result = sum / ( __int128 )n;

    /* apply the alternating sign factor (-1)^(n-1) */
    if ((n & 1) == 0) {      /* even n → negative sign */
        result = -result;
    }

    return result;
}
