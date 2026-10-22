/*
 * A351114  Characteristic function of balanced numbers: a(n) = 1 if phi(n)
 *          divides sigma(n), otherwise 0.
 * https://oeis.org/A351114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff φ(n) divides σ(n), otherwise 0
 * Range:     u8 n <= 97, u16 n <= 9571, u32 n <= 9571, u64 n <= 9571,
 *            u128 n <= 9571, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)            /* φ(1)=1, σ(1)=1 – balanced */
        return 1;

    seq_t phi   = (seq_t)n;   /* φ(n) */
    seq_t sigma = 1;          /* σ(n) */

    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0)
            continue;

        /* prime factor p found, count its exponent e */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* update φ(n) = φ(n)/p * (p-1) */
        phi = phi / p * (seq_t)(p - 1);

        /* compute the σ-part for this prime:
           term = (p^(e+1)-1)/(p-1)                           */
        seq_t pe = 1;
        for (idx_t i = 0; i < e + 1; ++i)
            pe = pe * (seq_t)p;
        seq_t term = (pe - 1) / (seq_t)(p - 1);
        sigma = sigma * term;
    }

    /* If the remaining temp is a prime > sqrt(original n) */
    if (temp > 1) {
        idx_t p = temp;
        phi = phi / p * (seq_t)(p - 1);
        seq_t term = (seq_t)(p + 1);  /* (p^2-1)/(p-1) = p+1 */
        sigma = sigma * term;
    }

    return ((sigma % phi) == 0) ? 1 : 0;
}
