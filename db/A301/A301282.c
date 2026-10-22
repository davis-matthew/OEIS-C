/*
 * A301282  Denominator of proportion of the volume of a unit box in R^4 that
 *          can be filled by n disjoint symplectically embedded balls of equal
 *          radius.
 * https://oeis.org/A301282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A301282 – Denominator of the proportion of the volume of a unit box
   in R^4 that can be filled by n disjoint symplectically embedded balls.
   The sequence is:
   a(1)=2, a(2)=1, a(3)=3, a(4)=9, a(5)=10, a(6)=49, a(7)=225,
   and for all n ≥ 8, a(n)=1.  This matches the known packing densities
   and the data in the OEIS b-file.  The implementation below is O(1)
   and independent of the size of seq_t.  */

/* External function required by the runner */
seq_t a(idx_t n)
{
    /* Handle the first few terms explicitly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)9;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)49;
    if (n == 7) return (seq_t)225;
    /* For n ≥ 8 the packing fraction is 1, so the denominator is 1 */
    return (seq_t)1;
}
