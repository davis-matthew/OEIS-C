/*
 * A294021  Number of elements added at n-th stage to the structure of the
 *          cellular automaton described in A294020.
 * https://oeis.org/A294021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = Σ_{m=0}^{⌊n/6⌋} p[n-6m] where the coefficients p[0..10]
 *            of the numerator of the rational generating function are p =
 *            {0,1,4,4,6,8,4,13,20,12,16}. For n > 10 the recurrence
 *            a(n)=a(n-6) automatically follows
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients of P(x)=x + 4x² + 4x³ + 6x⁴ + 8x⁵ + 4x⁶
       + 13x⁷ + 20x⁸ + 12x⁹ + 16x¹⁰ */
    static const seq_t p[] = {
        (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)4, (seq_t)6,
        (seq_t)8, (seq_t)4, (seq_t)13, (seq_t)20, (seq_t)12, (seq_t)16
    };
    const idx_t max_k = 10;          /* largest index with a non‑zero coefficient */
    const idx_t max_m = n / 6;       /* ⌊n/6⌋ */
    seq_t sum = 0;

    for (idx_t m = 0; m <= max_m; ++m) {
        idx_t k = n - 6 * m;        /* term to add */
        if (k <= max_k) {
            sum += p[k];
        }
    }
    return sum;
}
