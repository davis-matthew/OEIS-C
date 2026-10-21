/*
 * A266461  Decimal representation of the n-th iteration of the "Rule 27"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 2, a(3) = 123, a(4) = 4, a(5) = 2039,
 *            a(n) = 19*a(n-2) - 50*a(n-4) + 32*a(n-6) for n >= 6 (Rule 27)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t base[6] = {(seq_t)1, (seq_t)5, (seq_t)2,
                                  (seq_t)123, (seq_t)4, (seq_t)2039};

    if (n < 6)
        return base[n];

    /* keep the last six computed terms */
    seq_t last[6];
    for (idx_t i = 0; i < 6; ++i)
        last[i] = base[i];

    for (idx_t i = 6; i <= n; ++i)
    {
        idx_t i2 = (i - 2) % 6;   /* a(i-2) */
        idx_t i4 = (i - 4) % 6;   /* a(i-4) */
        idx_t i6 = (i - 6) % 6;   /* a(i-6) */

        seq_t val = (seq_t)19 * last[i2] - (seq_t)50 * last[i4]
                    + (seq_t)32 * last[i6];

        last[i % 6] = val;   /* store a(i) */
    }

    return last[n % 6];
}
