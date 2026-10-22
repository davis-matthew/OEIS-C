/*
 * A344721  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^3.
 * https://oeis.org/A344721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (-1)^(k+1) * ⌊n/k⌋^3
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 1682, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a sequence A344721: sum of alternating cubes of floor divisions. */

seq_t a(idx_t n)
{
    seq_t pos = 0;   /* Sum of terms with odd k */
    seq_t neg = 0;   /* Sum of terms with even k */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;        /* floor(n/k) */
        seq_t cube = (seq_t)q * q * q;

        if (k & 1U) {
            pos += cube;       /* k odd -> plus */
        } else {
            neg += cube;       /* k even -> minus */
        }
    }

    return pos - neg;          /* final result (positively valued) */
}
