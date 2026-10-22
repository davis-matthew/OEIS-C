/*
 * A351273  Sum of the 10th powers of the squarefree divisors of n.
 * https://oeis.org/A351273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^10 * mu(d)^2 = ∏_{p|n} (1 + p^10)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 84, u128 n <= 7132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A351273 */
seq_t a(idx_t n)
{
    if (n == 0) {               /* undefined for 0, return 0 as safe */
        return 0;
    }

    seq_t result = 1;           /* product over distinct primes */
    idx_t temp = n;

    /* trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* p is a prime factor of n */
            seq_t p_seq = p;
            seq_t p10 = 1;
            for (int i = 0; i < 10; ++i) {
                p10 *= p_seq;   /* compute p^10 */
            }
            seq_t factor = 1 + p10;   /* 1 + p^10 */
            result *= factor;      /* multiply into running product */

            /* remove all occurrences of p from temp */
            while (temp % p == 0) {
                temp /= p;
            }
        }
    }

    /* if temp > 1 then temp itself is a prime factor */
    if (temp > 1) {
        seq_t p_seq = temp;
        seq_t p10 = 1;
        for (int i = 0; i < 10; ++i) {
            p10 *= p_seq;
        }
        result *= (seq_t)(1 + p10);
    }

    return result;
}
