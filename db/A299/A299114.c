/*
 * A299114  Number of sides of a face of an Archimedean solid.
 * https://oeis.org/A299114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of sides of a face of an Archimedean solid.
   The sequence consists of the finite set {3,4,5,6,8,10} ordered
   increasingly. For indices beyond this set a value of 0 is returned. */
seq_t a(idx_t n) {
    /* Check bounds: offset is 1; a(1)=3, a(2)=4, a(3)=5, a(4)=6,
       a(5)=8, a(6)=10.  All other indices return 0. */
    /* We avoid a static lookup table by using explicit conditions. */
    if (n == 1) return 3;
    if (n == 2) return 4;
    if (n == 3) return 5;
    if (n == 4) return 6;
    if (n == 5) return 8;
    if (n == 6) return 10;
    return 0;  /* padding for undefined indices */
}
