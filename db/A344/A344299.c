/*
 * A344299  Expansion of Sum_{k>=1} (-1)^(k+1) * x^(k^2) / (1 - x^(k^2)).
 * https://oeis.org/A344299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p==2 ? 1 - ⌊e/2⌋ : 1 + ⌊e/2⌋)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function: a(2^e)=1-⌊e/2⌋, a(p^e)=1+⌊e/2⌋ for odd p. */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t half = (seq_t)(e / 2);
            seq_t factor = (p == 2) ? ((seq_t)1 - half) : ((seq_t)1 + half);
            result *= factor;
        }
    }

    /* Remaining prime factor, if any (exponent = 1) */
    if (temp > 1) {
        seq_t half = (seq_t)(1 / 2);   /* 0, but kept for symmetry */
        seq_t factor = (temp == 2) ? ((seq_t)1 - half) : ((seq_t)1 + half);
        result *= factor;
    }

    return result;
}
