/*
 * A300403  Smallest integer i such that SSCG(i) >= n.
 * https://oeis.org/A300403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the smallest i such that SSCG(i) >= n.
   Known SSCG values: SSCG(0)=2, SSCG(1)=5, SSCG(2)=3*2^(3*2^95)-8 (huge).
   Consequently, for all practical n: a(n)=0 if n<=2, a(n)=1 if 3<=n<=5,
   a(n)=2 if n>=6. */
seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    if (n <= 5)
        return 1;
    return 2;
}
