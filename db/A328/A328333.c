/*
 * A328333  Expansion of (1 + 4*x - 6*x^2) / ((1 - x) * (1 - 10*x^2)).
 * https://oeis.org/A328333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 9 Subsequent terms satisfy a(n) =
 *            a(n‑1) + 10·a(n‑2) – 10·a(n‑3) (derived from the rational
 *            generating function G(x) = (1 + 4x – 6x²) / ((1 – x)(1 – 10x²))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)5;
    seq_t a2 = (seq_t)9;
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t next = a2 + (seq_t)10 * a1 - (seq_t)10 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
