/*
 * A371042  E.g.f. satisfies A(x) = 1 + x^2*exp(x*A(x)).
 * https://oeis.org/A371042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} k^(n-2*k) * *
 *            binomial(n-2*k+1,k) / ((n-2*k+1)*(n-2*k)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* exponentiation by squaring for arbitrary-precision seq_t */
static seq_t seq_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* multiplicative binomial coefficient: binom(n,k) */
static seq_t binom_multiplicative(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* compute n! */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i) fact_n *= (seq_t)i;

    /* factor for (n-2k)! will be updated iteratively */
    seq_t fact_m = fact_n;
    idx_t m = n;

    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t nk = n - 2 * k;           /* n-2k */
        /* skip the (k==0 && nk>0) case, term is zero because 0^positive = 0 */
        if (!(k == 0 && nk > 0)) {
            seq_t bin = binom_multiplicative(nk + 1, k);
            /* compute k^(nk) */
            seq_t powk = seq_pow_seq((seq_t)k, nk);
            seq_t temp = fact_n;
            temp /= (seq_t)(nk + 1);
            temp /= fact_m;               /* fact_m == (n-2k)! */
            temp *= bin;
            temp *= powk;
            sum += temp;
        }
        /* update fact_m for next k (factorial of n-2*(k+1)) */
        if (k < maxk) {
            /* divide by m*(m-1) where m = n-2k */
            fact_m /= (seq_t)m;
            fact_m /= (seq_t)(m - 1);
            m -= 2;
        }
    }
    return sum;
}
