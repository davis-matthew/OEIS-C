/*
 * A284960  Number of elements of order n in the simple unitary group U3(4).
 * https://oeis.org/A284960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in the simple unitary group
 *            U3(4)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only the orders listed below occur in U3(4).  Sum of all non‑trivial counts
       together with 1 (the identity) equals the group order 62400. */
    switch (n) {
        case 1:  return 1;
        case 2:  return 195;
        case 3:  return 4160;
        case 4:  return 3900;
        case 5:  return 5824;
        case 10: return 12480;
        case 13: return 19200;
        case 15: return 16640;
        default: return 0;
    }
}
