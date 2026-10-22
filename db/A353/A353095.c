/*
 * A353095  a(1) = 3; for n > 1, a(n) = 4*a(n-1) + 4 - n.
 * https://oeis.org/A353095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; for n > 1, a(n) = 4*a(n-1) + 4 - n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return a(n) for the OEIS sequence A353095.  The implementation
 * follows the recurrence and works for any order of calls.  No
 * dynamic memory allocation is required, so it is valid for both
 * fixed‑width and arbitrary‑precision builds. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    seq_t val = 3;                  /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        val = 4 * val + 4 - (seq_t)i;
    }
    return val;
}
