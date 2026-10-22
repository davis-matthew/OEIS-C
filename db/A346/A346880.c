/*
 * A346880  Sum of the divisors, except the smallest and the largest, of the
 *          n-th positive even number.
 * https://oeis.org/A346880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2*n) - 1 - 2*n where sigma(x) is the sum of all
 *            positive divisors of x
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of a 64‑bit unsigned integer.
   The algorithm performs a trial division factorisation, using
   only 64‑bit arithmetic.  For each prime factor p^e we multiply
   the accumulated result by (p^(e+1)-1)/(p-1).  The result is
   returned in type seq_t. */
static seq_t sigma_u64(idx_t x)
{
    if (x <= 1)
        return (seq_t)x;   /* sigma(0)=0, sigma(1)=1 */

    seq_t result = 1;
    idx_t temp   = x;

    /* handle factor 2 separately */
    if ((temp & 1) == 0) {
        seq_t sum = 1;
        seq_t pk  = 1;
        while ((temp & 1) == 0) {
            temp >>= 1;
            pk  *= 2;
            sum += pk;
        }
        result *= sum;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t sum = 1;
            seq_t pk  = 1;
            while (temp % p == 0) {
                temp /= p;
                pk  *= (seq_t)p;
                sum += pk;
            }
            result *= sum;
        }
    }

    /* remaining prime factor > sqrt(original x) */
    if (temp > 1)
        result *= ((seq_t)1 + (seq_t)temp);

    return result;
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    idx_t m = n * 2;                     /* the n‑th even number */
    seq_t sigma = sigma_u64(m);
    return sigma - (seq_t)1 - (seq_t)m;   /* exclude 1 and m */
}
