/*
 * A378965  Semiperimeter of the unique primitive Pythagorean triple (x,y,z)
 *          such that (x-y+z)/2 is A002315(n) and such that its long leg and
 *          its hypotenuse are consecutive natural numbers.
 * https://oeis.org/A378965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002315(n) * (2*A002315(n) – 1) A002315 is defined by
 *            a(0)=1, a(1)=7, a(n)=6*a(n‑1) – a(n‑2) for n≥2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        seq_t m = (seq_t)1;
        return m * (((seq_t)2 * m) - 1);
    }
    if (n == 1) {
        seq_t m = (seq_t)7;
        return m * (((seq_t)2 * m) - 1);
    }

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)7;  /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = ((seq_t)6 * prev1) - prev2;  /* a(i) = 6*a(i‑1) – a(i‑2) */
        prev2 = prev1;
        prev1 = cur;
    }

    seq_t m = cur;
    return m * (((seq_t)2 * m) - 1);
}
