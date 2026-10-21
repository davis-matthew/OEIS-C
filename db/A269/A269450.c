/*
 * A269450  a(n) = (n-1)*a(n-1) - a(n-2) + (n-2)*a(n-3) with a(0)=0,
 *          a(1)=a(2)=1.
 * https://oeis.org/A269450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A269450
   a(0)=0, a(1)=1, a(2)=1
   a(n) = (n-1)*a(n-1) - a(n-2) + (n-2)*a(n-3)   for n >= 3 */
seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0u) return (seq_t)0;
    if (n <= 2u) return (seq_t)1;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)0;   /* a(0)   */
    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)1;   /* a(2)   */

    /* Compute iteratively up to the requested index n. */
    for (idx_t i = 3u; i <= n; ++i)
    {
        seq_t part1 = (seq_t)(i - 1u) * a2;          /* (i-1)*a(i-1) */
        seq_t part2 = (seq_t)(i - 2u) * a0;          /* (i-2)*a(i-3) */
        seq_t val   = part1 + part2;                /* add (i-2)*a(i-3) */
        val -= a1;                                  /* subtract a(i-2) */

        /* Shift for next iteration: a(n-2)=a1, a(n-1)=a2, a(n)=val. */
        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return a2;
}
