/*
 * A247397  Numbers k such that when k unit-diameter circles are arranged
 *          non-overlapping in the plane, and those circles are then enclosed
 *          in a rectangle, the area of the rectangle must be at least k.
 * https://oeis.org/A247397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247397: Numbers k such that any arrangement of k unit‑diameter
   circles enclosed in a rectangle must have area at least k.
   For k ≥ 14 a two‑row offset packing yields area < k, so the sequence
   ends with 13.  The first twelve terms are:

   a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7, a(8)=8,
   a(9)=9, a(10)=10, a(11)=12, a(12)=13. */

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 for safety */
    if (n < 1)
        return (seq_t)0;

    if (n <= 10)
        return (seq_t)n;

    if (n == 11)
        return (seq_t)12;

    if (n == 12)
        return (seq_t)13;

    /* the sequence has no terms beyond index 12 */
    return (seq_t)0;
}
