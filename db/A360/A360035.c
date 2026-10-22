/*
 * A360035  Expansion of e.g.f. x*exp(x)*cosh(x)*sinh(x).
 * https://oeis.org/A360035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^(n-1) - (-1)^(n-1))/4 Recurrence: let b(0)=0; for
 *            k≥1 b(k) = 3*b(k-1) + (-1)^(k-1) then a(n) = n*b(n-1) for n>0,
 *            and a(0)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t b = 0;
    for (idx_t k = 1; k <= n-1; ++k)
        b = 3 * b + ((k & 1U) ? 1 : -1);
    return n * b;
}
