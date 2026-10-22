/*
 * A351245  a(n) = n^5 * Sum_{p|n, p prime} 1/p^5.
 * https://oeis.org/A351245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p|n, p prime} (n/p)^5
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 167, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term (n/p)^5 is multiplied by 1 since the summand is n^5/p^5 = (n/p)^5. */

#include <stdint.h>
#include <stdbool.h>

/* Return a(n) for n >= 1.  a(1) = 0.  */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = 0;
    uint64_t temp = n;
    uint64_t nn   = n;

    /* Enumerate distinct prime divisors by trial division. */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Compute (n/p)^5 using only seq_t arithmetic. */
            seq_t x = (seq_t)(nn / p);
            seq_t term = x * x;   /* x^2   */
            term = term * x;      /* x^3   */
            term = term * x;      /* x^4   */
            term = term * x;      /* x^5   */

            sum += term;

            /* Remove all copies of p from temp to keep distinctness. */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If a prime factor > sqrt(n) remains, it appears once. */
    if (temp > 1) {
        seq_t x = (seq_t)(nn / temp);
        seq_t term = x * x;
        term = term * x;
        term = term * x;
        term = term * x;
        sum += term;
    }

    return sum;
}
