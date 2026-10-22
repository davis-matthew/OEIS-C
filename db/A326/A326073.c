/*
 * A326073  a(n) = gcd(1+n-A020639(n), 1+sigma(n)-A020639(n)-n), where A020639
 *          gives the smallest prime factor of n (and 1 for 1), and sigma is
 *          the sum of divisors of n.
 * https://oeis.org/A326073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(1+n-A020639(n), 1+sigma(n)-A020639(n)-n)
 * Range:     u8 n <= 175, u16 n <= 9216, u32 n <= 9216, u64 n <= 9216,
 *            u128 n <= 9216, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Smallest prime factor: 1 for n=1, otherwise the smallest
 * prime dividing n. */
static seq_t spf(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    for (idx_t p = 2; p * p <= n; ++p)
        if (n % p == 0) return (seq_t)p;
    return (seq_t)n;
}

/* Sum of divisors sigma(n).  Computed by prime factorisation. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; )
    {
        if (temp % p != 0) { ++p; continue; }

        /* exponent of p in n */
        idx_t exp = 0;
        while (temp % p == 0) { temp /= p; ++exp; }

        /* term = 1 + p + p^2 + ... + p^exp */
        seq_t term = 1;
        seq_t pow  = 1;
        for (idx_t k = 0; k < exp; ++k) {
            pow  = pow * (seq_t)p;
            term = term + pow;
        }
        sum = sum * term;
    }
    if (temp > 1)          /* remaining prime factor */
        sum = sum * ((seq_t)1 + (seq_t)temp);
    return sum;
}

/* Main sequence function.  We compute the two terms as signed
 * integers to handle the possible negative value for the second term,
 * take absolute values, and then apply the gcd routine that expects
 * unsigned seq_t arguments. */
seq_t a(idx_t n)
{
    seq_t p   = spf(n);
    seq_t sig = sigma_of(n);

    int64_t t1 = (int64_t)n + 1 - (int64_t)p;
    int64_t t2 = (int64_t)sig + 1 - (int64_t)p - (int64_t)n;

    if (t1 < 0) t1 = -t1;
    if (t2 < 0) t2 = -t2;

    return gcd((seq_t)t1, (seq_t)t2);
}
