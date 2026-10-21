/*
 * A269555  Expansion of (x^2 + 254*x - 7)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A269555: a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) for n≥3, with
 *            a(0)=7, a(1)=439, a(2)=42767
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)439;
    if (n == 2) return (seq_t)42767;

    seq_t a0 = (seq_t)7;
    seq_t a1 = (seq_t)439;
    seq_t a2 = (seq_t)42767;
    seq_t an = a2; /* placeholder for result */

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)99 * a2 - (seq_t)99 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
