/*
 * A301623  Numbers not divisible by 2, 3 or 5 (A007775) with digital root 5.
 * https://oeis.org/A301623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base[(n-1) mod 4] + 90 * floor((n-1)/4), base = {23, 41,
 *            59, 77}
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n < 1 the behaviour does not matter. */
    if (n == 0) return 0;

    const seq_t residues[4] = {23, 41, 59, 77};

    idx_t r     = (n - 1) & 3;          /* (n-1) % 4 */
    idx_t cycle = (n - 1) >> 2;         /* (n-1) / 4 */

    return residues[r] + (seq_t)cycle * (seq_t)90;
}
