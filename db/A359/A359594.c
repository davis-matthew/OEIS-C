/*
 * A359594  Multiplicative with a(p^e) = p^e if p divides e, 1 otherwise.
 * https://oeis.org/A359594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = multiplicative; for each prime power p^e in n, a(p^e) =
 *            p^e if p divides e, otherwise 1
 * Range:     u8 n <= 255, u16 n <= 9973, u32 n <= 9973, u64 n <= 9973,
 *            u128 n <= 9973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external utilities required. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1; behaviour for n==0 irrelevant */

    idx_t m = n;            /* mutable copy for factorisation */
    seq_t result = 1;       /* accumulate multiplicative contribution */

    /* trial division for prime factorisation */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            /* count exponent e of prime p */
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            /* include p^e only if p divides e */
            if (e % (int)p == 0)
            {
                /* compute p^e as seq_t */
                seq_t power = 1;
                for (int i = 0; i < e; ++i)
                {
                    power *= (seq_t)p;
                }
                result *= power;
            }
        }
    }

    /* if m > 1, the remaining factor is prime with exponent 1 */
    /* 1%prime != 0, so it contributes 1, nothing to do */

    return result;
}
