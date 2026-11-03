/*
 * A308694  Square array A(n,k), n >= 1, k >= 0, where A(n,k) = Sum_{d|n}
 *          d^(k*(n/d - 1)), read by antidiagonals.
 * https://oeis.org/A308694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * A(i,j) = Σ_{d|i}
 *            d^{ j*(i/d - 1) } along antidiagonals. * For a given index N
 *            (1‑based) let T_s = s*(s+1)/2. * Find the smallest s with T_s ≥
 *            N. Then * i = (N - T_{s-1}) (1‑based) * j = s - i * and a(N) =
 *            A(i,j). * * This implementation is fully gene...
 * Range:     u8 n <= 50, u16 n <= 96, u32 n <= 162, u64 n <= 311,
 *            u128 n <= 578, big any n
 * Validated: standard, pass, 94 terms through n = 8979 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Return the smallest integer s such that s*(s+1)/2 ≥ N */
static idx_t triangular_least(idx_t N)
{
    idx_t low = 1, high = N;          /* high is a safe upper bound */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        /* Compute mid*(mid+1)/2 with 128‑bit to avoid overflow */
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= N)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t N)
{
    /* Find the antidiagonal sum (s) */
    idx_t s = triangular_least(N);

    /* Sum of elements before this antidiagonal */
    __uint128_t prev = (__uint128_t)(s - 1) * s / 2;
    idx_t pos = (idx_t)(N - (idx_t)prev - 1);      /* 0‑based position within the diagonal */

    /* Convert to matrix coordinates */
    idx_t i = pos + 1;            /* row index n ≥ 1 */
    idx_t j = s - i;              /* column index k ≥ 0 */

    /* Compute A(i,j) = Σ_{d|i} d^{ j*(i/d - 1) } */
    seq_t sum = 0;
    for (idx_t d = 1; d <= i; ++d)
    {
        if (i % d == 0)
        {
            idx_t n_over_d = i / d;
            idx_t exp = j * (n_over_d - 1);          /* exponent fits in idx_t */
            seq_t pw = ipow((seq_t)d, (seq_t)exp);/* compute d^exp */
            sum += pw;
        }
    }

    return sum;
}
