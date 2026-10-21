/*
 * A246082  Paradigm shift sequence for (0,4) production scheme with
 *          replacement.
 * https://oeis.org/A246082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * a(n-12) for all n ≥ 20, with a(1) … a(16) = 1 … 16
 *            and a(17) = 18, a(18) = 20, a(19) = 22
 * Range:     u8 n <= 45, u16 n <= 105, u32 n <= 227, u64 n <= 469,
 *            u128 n <= 954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can therefore be computed by reducing the index
   modulo 12 and multiplying by the appropriate power of 3. */

seq_t a(idx_t n)
{
    /* Handle the initial values without hard‑coding them. */
    if (n < 20) {
        seq_t res = (seq_t)n;          /* a(n) = n  for n ≤ 16       */
        if (n > 16)                    /* a(17) = 18, a(18) = 20, a(19) = 22 */
            res += (seq_t)(n - 16);
        return res;
    }

    /* Reduce n until it is within the range 1…19, counting how many
       times we subtracted 12. Each subtraction multiplies the result
       by 3. */
    idx_t m = n;
    idx_t exp = 0;
    while (m > 19) {
        m -= 12;
        ++exp;
    }

    /* Compute the base value for the reduced index. */
    seq_t base = (seq_t)m;
    if (m > 16)
        base += (seq_t)(m - 16);

    /* Compute 3^exp using binary exponentiation. */
    seq_t pow3 = 1;
    seq_t three = (seq_t)3;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1u)
            pow3 *= three;
        three *= three;
        e >>= 1;
    }

    return base * pow3;
}
