/*
 * A247504  Square array read by antidiagonals, A(n,k) =
 *          k!*[x^k]((1-Sum_{j=1..n} x^j)^(-1)), (n>=0,k>=0).
 * https://oeis.org/A247504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247504: square array read by antidiagonals.
   A(n,k) = k! * coeff of x^k in (1 - Σ_{j=1..n} x^j)^(-1).
   The sequence a(n) outputs the n‑th element of that array read
   by antidiagonals: (0,0),(1,0),(0,1),(2,0),(1,1),(0,2),…   */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Find the antidiagonal d so that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    uint64_t low = 0;
    uint64_t high = (uint64_t)(sqrtl((long double)n * 2.0L)) + 2; /* safe upper bound */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t d = low;
    uint64_t base = d * (d + 1) / 2;             /* triangular number of d */
    uint64_t offset = n - base;                  /* position on the d‑th diagonal */
    uint64_t i = d - offset;                     /* row index (n in the array) */
    uint64_t j = offset;                         /* column index (k in the array) */

    /* Compute coefficient of x^j in 1/(1 - Σ_{t=1..i} x^t) via DP */
    size_t jsz = (size_t)j;
    seq_t *coeff = (seq_t*)malloc((jsz + 1) * sizeof(seq_t));
    if (!coeff) return 0;                        /* memory failure – unlikely here */
    coeff[0] = 1;
    for (size_t k = 1; k <= jsz; ++k) {
        seq_t sum = 0;
        size_t maxstep = (size_t)(i < k ? i : k);
        for (size_t step = 1; step <= maxstep; ++step)
            sum += coeff[k - step];
        coeff[k] = sum;
    }

    /* Compute factorial j! */
    seq_t fact = 1;
    for (size_t f = 1; f <= jsz; ++f)
        fact *= (seq_t)f;

    seq_t result = coeff[jsz] * fact;
    free(coeff);
    return result;
}
