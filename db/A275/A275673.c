/*
 * A275673  List of numbers that are in a spoke of a hexagonal spiral.
 * https://oeis.org/A275673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+4-3*floor((n+4)/6)-2)*floor((n+4)/6)+1
 * Range:     u8 n <= 53, u16 n <= 884, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Protect against an out‑of‑range index (not part of the OEIS
       definition).  For n == 0 we simply return 0; otherwise we compute
       the formula directly. */
    if (n == 0)
        return 0;

    /* n + 4 and its division by 6 (floor). */
    seq_t n_plus_4 = (seq_t)n + 4;
    seq_t div6     = n_plus_4 / 6;

    /* (n+4 - 3*div6 - 2) * div6 + 1 */
    seq_t term = n_plus_4 - 3 * div6 - 2;
    return term * div6 + 1;
}
