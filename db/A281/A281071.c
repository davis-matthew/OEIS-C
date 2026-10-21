/*
 * A281071  Largest number k such that b - r is even or r = 0 for all b = 1..k
 *          where r = n mod b.
 * https://oeis.org/A281071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property-based algorithm:  a(n) is the largest k such that
   for all b = 1 .. k, either n mod b = 0 or parity(b) = parity(n mod b). */

seq_t a(idx_t n)
{
    /* Iterate over possible screen widths b.  We stop when we find a
       violation of the condition above.  For odd n the very first check
       at b = 2 fails, so a(n) = 1.  For even n we continue until the
       first odd b with an even, non–zero remainder occurs.  Then k = b–1.
       If no violation is found up to n, the largest admissible width is n.
       (For odd b > n, the remainder equals n itself which is even,
       thus a violation would occur immediately after n.)  */
    for (idx_t b = 1; b <= n; ++b) {
        idx_t r = n % b;
        if (r != 0) {
            /* r is non‑zero; check parity mismatch.  For odd b we
               need an odd remainder, for even b an even remainder. */
            if ( (b & 1) != (r & 1) )
                return (seq_t)(b - 1);
        }
    }
    /* No violation found up to n; return n itself. */
    return (seq_t)n;
}
