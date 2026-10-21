/*
 * A275546  a(n) = (tan(1*Pi/11))^(2*n) + (tan(2*Pi/11))^(2*n) +
 *          (tan(3*Pi/11))^(2*n) + (tan(4*Pi/11))^(2*n) +
 *          (tan(5*Pi/11))^(2*n).
 * https://oeis.org/A275546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..5} (tan(k·π/11))^(2n) Recurrence: a(n) =
 *            55·a(n‑1) – 330·a(n‑2) + 462·a(n‑3) – 165·a(n‑4) + 11·a(n‑5) for
 *            n ≥ 5 Initial terms: a(0)=5, a(1)=55, a(2)=2365, a(3)=113311,
 *            a(4)=5476405
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    static const seq_t init[5] = {5, 55, 2365, 113311, 5476405};

    if (n < 5)
        return init[n];

    /* sliding window for last five computed values */
    seq_t a0 = init[0], a1 = init[1], a2 = init[2],
          a3 = init[3], a4 = init[4];

    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence with positive 11 coefficient */
        seq_t next = a4 * (seq_t)55
                   - a3 * (seq_t)330
                   + a2 * (seq_t)462
                   - a1 * (seq_t)165
                   + a0 * (seq_t)11;  /* +11 * a(n‑5) */

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
