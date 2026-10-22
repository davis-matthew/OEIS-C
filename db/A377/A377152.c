/*
 * A377152  a(n) = Sum_{k=0..n} binomial(k+4,4) * binomial(k,n-k)^2.
 * https://oeis.org/A377152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+4,4) * binomial(k,n-k)^2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) in O(n) time using recurrence relations for the binomial terms. */
#include <stdint.h>

/* Helper: compute binomial(x,4) for unsigned integer x (x>=4). */
static seq_t binom4(idx_t x)
{
    /* binom(x,4) = x*(x-1)*(x-2)*(x-3)/24 */
    seq_t res = (seq_t)(x) * (seq_t)(x-1);
    res = res * (seq_t)(x-2);
    res = res * (seq_t)(x-3);
    /* 24 = 3*2*4 */
    res = res / (seq_t)24;
    return res;
}

seq_t a(idx_t n)
{
    /* Use t = n - k, iterate t = 0 .. floor(n/2) */
    const idx_t limit = n / 2;   /* integer division */
    seq_t sum = 0;

    /* A_t = binom(n-t, t) ; start with t=0 */
    seq_t A = 1;

    /* B_t = binom(n-t+4, 4) ; start with t=0 */
    seq_t B = binom4(n + 4);

    for (idx_t t = 0; t <= limit; ++t)
    {
        /* term = A^2 * B */
        seq_t temp = A * A;
        sum += temp * B;

        if (t == limit) break;     /* no further updates needed */

        /* Update A for next t: A_{t+1} = A * (n-2t)(n-2t-1) / ((t+1)(n-t)) */
        seq_t numA = (seq_t)(n - 2*t) * (seq_t)(n - 2*t - 1);
        seq_t denA = (seq_t)(t + 1) * (seq_t)(n - t);
        A = (A * numA) / denA;

        /* Update B for next t: B_{t+1} = B * ( (n-t+4)-4 ) / (n-t+4) */
        seq_t m = (seq_t)(n - t + 4);
        B = (B * (m - 4)) / m;
    }

    return sum;
}
