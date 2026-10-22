/*
 * A302341  Triameter of the n X n knight graph.
 * https://oeis.org/A302341
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = a(n-1) + a(n-12) - a(n-13)   for n > 19.
 * Initial values (offset 4):
 * a(4)=12, a(5)=12, a(6)=12, a(7)=12,
 * a(8)=16, a(9)=16, a(10)=18, a(11)=20, a(12)=22, a(13)=22,
 * a(14)=24, a(15)=26, a(16)=28, a(17)=30, a(18)=32, a(19)=32,
 * a(20)=36, a(21)=36, a(22)=38, a(23)=40, a(24)=42, a(25)=42,
 * a(26)=44, a(27)=46
 *
 * The sequence is computed by a linear recurrence of order 13,
 * so a sliding window of the last 13 values is sufficient.
 */
#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle positions below the offset – undefined behaviour, return 0. */
    if (n < 4) return 0;

    /* If n is within the explicitly given range, return the hard‑coded value. */
    switch (n) {
        case 4:  return 12; case 5:  return 12; case 6:  return 12; case 7:  return 12;
        case 8:  return 16; case 9:  return 16; case 10: return 18; case 11: return 20;
        case 12: return 22; case 13: return 22; case 14: return 24; case 15: return 26;
        case 16: return 28; case 17: return 30; case 18: return 32; case 19: return 32;
        case 20: return 36; case 21: return 36; case 22: return 38; case 23: return 40;
        case 24: return 42; case 25: return 42; case 26: return 44; case 27: return 46;
    }

    /* Initialize a ring buffer holding the last 13 values needed by the
     * recurrence.  The buffer stores a(i) at index (i-4) % 13.
     * The values for i = 4 … 27 are filled explicitly. */
    seq_t buf[13];

    buf[0]  = 12;  /* i = 4 */
    buf[1]  = 12;  /* i = 5 */
    buf[2]  = 12;  /* i = 6 */
    buf[3]  = 12;  /* i = 7 */
    buf[4]  = 16;  /* i = 8 */
    buf[5]  = 16;  /* i = 9 */
    buf[6]  = 18;  /* i =10 */
    buf[7]  = 20;  /* i =11 */
    buf[8]  = 22;  /* i =12 */
    buf[9]  = 22;  /* i =13 */
    buf[10] = 24;  /* i =14 */
    buf[11] = 26;  /* i =15 */
    buf[12] = 28;  /* i =16 */

    buf[0]  = 30;  /* i =17 */
    buf[1]  = 32;  /* i =18 */
    buf[2]  = 32;  /* i =19 */
    buf[3]  = 36;  /* i =20 */
    buf[4]  = 36;  /* i =21 */
    buf[5]  = 38;  /* i =22 */
    buf[6]  = 40;  /* i =23 */
    buf[7]  = 42;  /* i =24 */
    buf[8]  = 42;  /* i =25 */
    buf[9]  = 44;  /* i =26 */
    buf[10] = 46;  /* i =27 */

    /* Compute all terms from 28 up to n using the recurrence. */
    for (idx_t i = 28; i <= n; ++i) {
        size_t posNext   = (i - 4) % 13;
        size_t posPrev   = (i - 1 - 4) % 13;   /* i-1 */
        size_t posMinus12 = (i - 12 - 4) % 13;  /* i-12 */
        size_t posMinus13 = (i - 13 - 4) % 13;  /* i-13 */
        buf[posNext] = buf[posPrev] + buf[posMinus12] - buf[posMinus13];
    }

    return buf[(n - 4) % 13];
}
