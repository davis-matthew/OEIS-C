/*
 * A355326  Determinant of the n X n matrix [(i-j)^3+d(i,j)]_{1<=i,j<=n},
 *          where d(i,j) is 1 or 0 according as i = j or not.
 * https://oeis.org/A355326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 1 + n^2*(n^2-1)*P(n^2)/672000, * where P(t) = t^6 -
 *            19*t^5 + 123*t^4 - 337*t^3 + 12376*t^2 - 44144*t + 40000. * The
 *            implementation evaluates this expression in integer arithmetic
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Return for the offset terms */
    if (n <= 1)  /* a(0) = 1, a(1) = 1 */
        return (seq_t)1;

    /* k = n^2 */
    seq_t k = (seq_t)n * (seq_t)n;

    /* term1 = k*(k-1) = n^2*(n^2-1) */
    seq_t term1 = k * (k - (seq_t)1);

    /* Compute powers of k up to k^6 */
    seq_t k2 = k * k;          /* k^2 */
    seq_t k3 = k2 * k;         /* k^3 */
    seq_t k4 = k3 * k;         /* k^4 */
    seq_t k5 = k4 * k;         /* k^5 */
    seq_t k6 = k5 * k;         /* k^6 */

    /* P(k) = k^6 - 19*k^5 + 123*k^4 - 337*k^3 + 12376*k^2 - 44144*k + 40000
     * To avoid unsigned underflow, compute sum of positive terms minus sum of negative terms. */
    seq_t pos = k6
        + (seq_t)123 * k4
        + (seq_t)12376 * k2
        + (seq_t)40000;
    seq_t neg = (seq_t)19 * k5
        + (seq_t)337 * k3
        + (seq_t)44144 * k;
    seq_t Pk = pos - neg;      /* a(n) fits in seq_t for small n */

    /* Divide term1 * Pk by 672000 = 2^8 * 3 * 5^3 * 7.
     * Perform the prime factor division on the two multiplicands separately
     * to keep intermediate values within seq_t limits. */
    seq_t num1 = term1;
    seq_t num2 = Pk;

    /* Divide by 2^8 */
    {
        int exp = 8;
        const seq_t p = (seq_t)2;
        while (exp > 0)
        {
            if (num1 % p == 0)
            {
                num1 /= p;
                exp--;
            }
            else if (num2 % p == 0)
            {
                num2 /= p;
                exp--;
            }
        }
    }

    /* Divide by 3^1 */
    {
        int exp = 1;
        const seq_t p = (seq_t)3;
        if (num1 % p == 0)
        {
            num1 /= p;
            exp--;
        }
        if (exp > 0 && num2 % p == 0)
        {
            num2 /= p;
            exp--;
        }
    }

    /* Divide by 5^3 */
    {
        int exp = 3;
        const seq_t p = (seq_t)5;
        while (exp > 0)
        {
            if (num1 % p == 0)
            {
                num1 /= p;
                exp--;
            }
            else if (num2 % p == 0)
            {
                num2 /= p;
                exp--;
            }
        }
    }

    /* Divide by 7^1 */
    {
        int exp = 1;
        const seq_t p = (seq_t)7;
        if (num1 % p == 0)
        {
            num1 /= p;
            exp--;
        }
        if (exp > 0 && num2 % p == 0)
        {
            num2 /= p;
            exp--;
        }
    }

    /* The remaining product is (term1 * Pk) / 672000. */
    seq_t product = num1 * num2;

    /* Final result = 1 + product */
    return (seq_t)1 + product;
}
