/*
 * A381195  Expansion of g.f. (1 - sqrt(1 - 1728*x))/(864*x).
 * https://oeis.org/A381195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n)+864*(-2*n+1)*a(n-1)=0
   initial values: a(0)=1, a(1)=432 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)864 * ((seq_t)(2 * i - 1));
        res = res * mult;
        res = res / (i + 1);
    }
    return res;
}
