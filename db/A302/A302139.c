/*
 * A302139  Numbers k such that A111336(k) = 4.
 * https://oeis.org/A302139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-3) for n ≥ 13; * the first twelve values are:
 *            8,12,16,20,24,32,64,120,128,256,512,600
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 13) {
        /* n is 1‑based */
        switch ((idx_t)n) {
            case 1:  return (seq_t)8;
            case 2:  return (seq_t)12;
            case 3:  return (seq_t)16;
            case 4:  return (seq_t)20;
            case 5:  return (seq_t)24;
            case 6:  return (seq_t)32;
            case 7:  return (seq_t)64;
            case 8:  return (seq_t)120;
            case 9:  return (seq_t)128;
            case 10: return (seq_t)256;
            case 11: return (seq_t)512;
            case 12: return (seq_t)600;
            default: return (seq_t)0;  /* should not be reached */
        }
    }
    return pow2(n - 3);    /* pow2 is defined in ../utils/pow2.h */
}
