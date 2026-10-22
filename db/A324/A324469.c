/*
 * A324469  Exponent of highest power of 3 that divides
 *          multinomial(4*n;n,n,n,n).
 * https://oeis.org/A324469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exponent of the highest power of 3 that divides
 *            multinomial(4*n; n,n,n,n) Equivalently a(n) =
 *            v3((4*n)!)-4*v3(n!), where v3(m!) = ∑_{k≥1} ⌊m/3^k⌋
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4*n may overflow idx_t for very large n, so use seq_t for all arithmetic. */
    seq_t four_n = (seq_t)n * (seq_t)4;   /* 4*n */
    seq_t sum    = 0;                     /* result accumulator */

    for (seq_t p = (seq_t)3; p <= four_n; p *= (seq_t)3) {
        /* term = ⌊4*n / p⌋ - 4*⌊n / p⌋  */
        seq_t term = (four_n / p) - ((seq_t)4 * ((seq_t)n / p));
        sum += term;
    }

    return sum;
}
