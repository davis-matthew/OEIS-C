/*
 * A340455  G.f.: Sum_{n>=0} x^(2*n)/(1 - x^(5*n+2)) - x*Sum_{n>=0} x^(3*n)/(1
 *          - x^(5*n+3)).
 * https://oeis.org/A340455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = # { (m,k) ≥ 0 | (2+5m)k + 2m = n } – # { (m,k) ≥ 0 |
 *            (3+5m)k + 3m + 1 = n } Computed directly by counting solutions
 *            to the two linear Diophantine equations derived from the given
 *            generating function
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count1 = 0;
    seq_t count2 = 0;

    /* First sum */
    idx_t m_limit1 = n / 2;
    for (idx_t m = 0; m <= m_limit1; ++m) {
        seq_t numer = (seq_t)n - ((seq_t)2 * (seq_t)m);
        seq_t denom = (seq_t)2 + ((seq_t)5 * (seq_t)m);
        if (numer % denom == 0)
            count1 += 1;
    }

    /* Second sum */
    if (n >= 1) {
        idx_t m_limit2 = (n - 1) / 3;
        for (idx_t m = 0; m <= m_limit2; ++m) {
            seq_t numer = (seq_t)n - (((seq_t)3 * (seq_t)m) + (seq_t)1);
            seq_t denom = (seq_t)3 + ((seq_t)5 * (seq_t)m);
            if (numer % denom == 0)
                count2 += 1;
        }
    }

    return count1 - count2;
}
