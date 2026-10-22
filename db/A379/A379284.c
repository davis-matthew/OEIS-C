/*
 * A379284  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^2) * (1 - x*A(x)^4)).
 * https://oeis.org/A379284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n+3*k+1,k) *
 *            binomial(3*n+k,n-k)/(2*n+3*k+1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

static inline seq_t mul_div_exact(seq_t a, seq_t b, seq_t d)
{
    seq_t g = gcd(a, d);
    a /= g;
    d /= g;
    g = gcd(b, d);
    b /= g;
    d /= g;
    /* after removing all common factors, d should be 1 */
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t A = 1; /* C(2n+1,0) */
    seq_t B = binomial((idx_t)(3*n), (idx_t)n); /* C(3n,n) */
    seq_t denom = (seq_t)(2*n + 1);
    seq_t term = mul_div_exact(A, B, denom);
    sum = term;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update A: A = C(2n+3k+1, k) */
        seq_t numA = (seq_t)(2*n + 3*k + 1) *
                     (seq_t)(2*n + 3*k) *
                     (seq_t)(2*n + 3*k - 1);
        seq_t denA = (seq_t)k *
                     (seq_t)(2*n + 2*k) *
                     (seq_t)(2*n + 2*k + 1);
        A = mul_div_exact(A, numA, denA);

        /* Update B: B = C(3n+k, n-k) */
        seq_t numB = (seq_t)(3*n + k) *
                     (seq_t)(n - k + 1);
        seq_t denB = (seq_t)(2*n + 2*k - 1) *
                     (seq_t)(2*n + 2*k);
        B = mul_div_exact(B, numB, denB);

        denom = (seq_t)(2*n + 3*k + 1);
        term = mul_div_exact(A, B, denom);
        sum += term;
    }

    return sum;
}
