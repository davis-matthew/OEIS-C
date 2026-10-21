/*
 * A247283  Positions of subrecords in A048673.
 * https://oeis.org/A247283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5, 7, 9, 15, 18, 27 for n = 1..6, a(n) = 2 * a(n-2) for n
 *            > 6
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1‑based table of the first six values (index 0 unused). */
    static const seq_t init[7] = {0, 5, 7, 9, 15, 18, 27};

    if (n <= 6)
        return init[n];

    /* We need a(n‑2) and a(n‑1) to compute a(n).  Initialise for i = 7. */
    seq_t prev2 = init[5];   /* a(5) */
    seq_t prev1 = init[6];   /* a(6) */
    seq_t a_i   = 0;         /* placeholder */

    for (idx_t i = 7; i <= n; ++i) {
        a_i   = prev2 + prev2;   /* a(i) = 2 * a(i‑2) */
        prev2 = prev1;           /* shift window for next step */
        prev1 = a_i;
    }

    return a_i;
}
