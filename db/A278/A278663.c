/*
 * A278663  Number of periodic necklaces with n beads of 3 colors.
 * https://oeis.org/A278663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n)*sum_{d|n} (phi(d)-mu(d))*3^(n/d)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute phi(n) for n>=1.  n is small (<= 10000) */
static seq_t compute_phi(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            result = result / p * (p - 1);
        }
    }
    if (tmp > 1)
        result = result / tmp * (tmp - 1);
    return result;
}

/* Compute the Möbius function of n (n>=1).  Returns -1, 0, or 1. */
static int compute_mu(idx_t n)
{
    int mu = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
                if (exp > 1)
                    return 0;      /* square factor -> mu = 0 */
            }
            mu = -mu;
        }
    }
    if (tmp > 1)
        mu = -mu;
    return mu;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* a(0) = 0 by definition */

    /* Pre‑compute 3^k for k = 0..n */
    seq_t *pow3 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow3)
        return 0;                  /* allocation failure → 0 (unlikely) */
    pow3[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    seq_t sum = 0;

    /* Enumerate divisors d of n by iterating i up to sqrt(n) */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* First divisor d1 = i */
        idx_t d1 = i;
        idx_t e1 = n / d1;          /* exponent in 3^e1 */
        seq_t term1 = pow3[e1];

        int mu1 = compute_mu(d1);
        seq_t phi1 = compute_phi(d1);
        seq_t diff1;
        if (mu1 < 0)
            diff1 = phi1 + 1;
        else if (mu1 > 0)
            diff1 = phi1 - 1;
        else
            diff1 = phi1;

        sum += term1 * diff1;

        /* Second divisor d2 = n/i, if distinct */
        idx_t d2 = n / i;
        if (d2 != d1) {
            idx_t e2 = n / d2;          /* this equals i */
            seq_t term2 = pow3[e2];

            int mu2 = compute_mu(d2);
            seq_t phi2 = compute_phi(d2);
            seq_t diff2;
            if (mu2 < 0)
                diff2 = phi2 + 1;
            else if (mu2 > 0)
                diff2 = phi2 - 1;
            else
                diff2 = phi2;

            sum += term2 * diff2;
        }
    }

    free(pow3);
    return sum / (seq_t)n;   /* exact division */
}
