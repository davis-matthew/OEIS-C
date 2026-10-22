/*
 * A359289  Number of divisors of 4*n-2 of form 4*k+1.
 * https://oeis.org/A359289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of (2*n‑1) that satisfy d ≡ 1 (mod
 *            4)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* m = 2*n - 1 is odd */
    idx_t m = 2 * n - 1;
    idx_t tmp = m;

    seq_t mult_q1   = 1;   /* product of (ai+1) for primes ≡ 1 (mod 4) */
    seq_t prod_S3   = 1;   /* product of (ai+1) for primes ≡ 3 (mod 4) */
    bool all_even   = true;  /* true iff every exponent of a ≡ 3 mod 4 prime is even */

    /* trial division by odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                exp++;
            }
            seq_t factor = exp + 1;  /* number of choices for this exponent */

            if ((p % 4) == 1)
                mult_q1 *= factor;
            else /* (p % 4) == 3 */
            {
                prod_S3 *= factor;
                if (exp & 1)   /* exp is odd */
                    all_even = false;
            }
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1)
    {
        idx_t p = tmp;
        seq_t factor = 2; /* exponent is 1 */

        if ((p % 4) == 1)
            mult_q1 *= factor;
        else /* (p % 4) == 3 */
        {
            prod_S3 *= factor;
            all_even = false; /* exponent 1 is odd */
        }
    }

    seq_t diff = all_even ? 1 : 0;           /* Π(diff_i) where diff_i = 1 if exponent even else 0 */
    seq_t count = mult_q1 * ((prod_S3 + diff) / 2);
    return count;
}
