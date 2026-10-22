/*
 * A386292  The maximum possible number of 9-cycles in an outerplanar graph on
 *          n vertices.
 * https://oeis.org/A386292
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 94 + 190·floor((n-16)/8) + sum of first ((n-16) mod 8)
 *            terms of * the period {15,22,20,30,19,26,24,34}. The first eight
 *            values (for n=9…16) * are given explicitly. This follows from
 *            the periodic first-differences * after n=16:
 *            15,22,20,30,19,26,24,34, repeating with period 8
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n < 9 (offset undefined, return 0 silently). */
    if (n < 9)
        return (seq_t)0;

    /* Explicit values for 9 ≤ n ≤ 16. */
    static const seq_t base[8] = {1, 5, 11, 23, 34, 52, 68, 94};
    if (n <= 16)
        return base[n - 9];

    /* Periodic part for n ≥ 17. */
    const seq_t cycle[8] = {15, 22, 20, 30, 19, 26, 24, 34};

    /* Use idx_t arithmetic for indices; cast to seq_t for value. */
    idx_t offset   = n - 16;          /* distance from n = 16 */
    idx_t cycles   = offset / 8;      /* number of full 8‑step cycles */
    idx_t remainder = offset % 8;     /* remaining steps in partial cycle */

    /* Compute result using 64‑bit (or larger) arithmetic. */
    seq_t result = (seq_t)94;
    result += (seq_t)190 * (seq_t)cycles;

    for (idx_t i = 0; i < remainder; ++i)
        result += cycle[i];

    return result;
}
