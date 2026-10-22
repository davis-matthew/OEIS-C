/*
 * A370187  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x+x^3)^2
 *          )^n.
 * https://oeis.org/A370187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,k) *
 *            binomial(4*n-k,n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Multiply res by a and divide by b using exact arithmetic.
 * Cancellation is performed before the multiplication to avoid overflow
 * on fixed-width seq_t types.
 */
static seq_t muldiv(seq_t res, idx_t a, idx_t b)
{
    seq_t A = (seq_t)a;
    seq_t B = (seq_t)b;

    /* cancel common factors of a and b */
    seq_t g = gcd(A, B);
    if (g > (seq_t)1) {
        A /= g;
        B /= g;
    }

    /* cancel factors of the remaining denominator with res */
    g = gcd(res, B);
    if (g > (seq_t)1) {
        res /= g;
        B /= g;
    }

    /* any leftover denominator must divide res now */
    if (B != (seq_t)1) {
        res /= B;
        B = (seq_t)1;
    }

    return res * A;
}

/* Compute binomial(N,K) using a multiplicative formula that
 * uses cancellation to keep intermediate values within bounds.
 * K <= N.  Works for both fixed-width and arbitrary-precision seq_t.
 */
static seq_t binomial_reduced(idx_t N, idx_t K)
{
    if (K == 0 || K == N)
        return (seq_t)1;

    if (K > N - K)
        K = N - K;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        result = muldiv(result, N - K + i, i);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    seq_t c2n = (seq_t)1;        /* binomial(2*n,0) */

    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0)    /* update binomial(2*n,k) from binomial(2*n,k-1) */
            c2n = muldiv(c2n, 2 * n - k + 1, k);

        idx_t N = 4 * n - k;
        idx_t K = n - 3 * k;
        seq_t t2 = binomial_reduced(N, K);
        sum += c2n * t2;
    }
    return sum;
}
