/*
 * A294015  Sum of the even divisors of 2n, minus the (n-1)st odd number.
 * https://oeis.org/A294015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000203(n) - 2*n + 1 * where A000203(n) is the sum of
 *            divisors of n (σ(n)). * a(n) = 2*σ(n) - 2*n + 1
 * Range:     u8 n <= 48, u16 n <= 9859, u32 n <= 9859, u64 n <= 9859,
 *            u128 n <= 9859, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of the calculation routine */
static seq_t sigma_of_u64(idx_t n);

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sigma_n = sigma_of_u64(n);
    seq_t two = (seq_t)2;
    return two * sigma_n - two * (seq_t)n + (seq_t)1;
}

/* Compute σ(n) = sum of positive divisors of n
 * Uses trial division; sufficient for n up to at least 10^6
 * on a modern machine (and works for larger n with more time). */
static seq_t sigma_of_u64(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;  /* σ(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Handle factor 2 separately */
    idx_t exp = 0;
    while (m % (idx_t)2 == (idx_t)0) {
        m /= (idx_t)2;
        exp++;
    }
    if (exp > (idx_t)0) {
        /* term = (2^(exp+1) - 1) / (2 - 1) = 2^(exp+1) - 1 */
        seq_t power = (seq_t)1;
        for (idx_t i = (idx_t)0; i < exp + (idx_t)1; i++)
            power *= (seq_t)2;
        seq_t term = power - (seq_t)1;
        result *= term;
    }

    /* Handle odd prime factors */
    for (idx_t p = (idx_t)3; p * p <= m; p += (idx_t)2) {
        if (m % p != (idx_t)0)
            continue;
        exp = (idx_t)0;
        while (m % p == (idx_t)0) {
            m /= p;
            exp++;
        }
        /* term = (p^(exp+1) - 1) / (p - 1) */
        seq_t power = (seq_t)1;
        for (idx_t i = (idx_t)0; i < exp + (idx_t)1; i++)
            power *= (seq_t)p;
        seq_t term = (power - (seq_t)1) / ((seq_t)p - (seq_t)1);
        result *= term;
    }

    /* If a prime factor remains > sqrt(original n) */
    if (m > (idx_t)1) {
        /* term = 1 + m */
        seq_t term = (seq_t)1 + (seq_t)m;
        result *= term;
    }

    return result;
}
