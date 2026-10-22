/*
 * A383739  Smallest number that, when displayed on a 7-segment display using
 *          A006942, leaves exactly n segments unused.
 * https://oeis.org/A383739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0)=8, a(1)=0, a(2)=2, a(3)=4, a(4)=7, a(5)=1; For p≥1: a(5p) =
 *            (10^p-1)/9; a(5p+1) = (91·10^(p-1)-1)/9; a(5p+2) =
 *            (10^(p+1)-1)/9 + 1; a(5p+3) = (10^(p+1)-1)/9 + 3; a(5p+4) =
 *            (10^(p+1)-1)/9 + 6;
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[5] = { (seq_t)8, (seq_t)0, (seq_t)2,
                                   (seq_t)4, (seq_t)7 };

    if (n < 5)            /* 0 … 4 */
        return init[n];

    idx_t p = n / 5;          /* floor(n/5)  */
    idx_t r = n % 5;          /* n mod 5     */
    seq_t res;

    switch (r) {
    case 0: /* a(5p)   */
        res = (pow10(p) - (seq_t)1) / 9;
        break;
    case 1: /* a(5p+1) */
        res = (((seq_t)91 * pow10(p - 1)) - (seq_t)1) / 9;
        break;
    case 2: /* a(5p+2) */
        res = ((pow10(p + 1) - (seq_t)1) / 9) + (seq_t)1;
        break;
    case 3: /* a(5p+3) */
        res = ((pow10(p + 1) - (seq_t)1) / 9) + (seq_t)3;
        break;
    case 4: /* a(5p+4) */
        res = ((pow10(p + 1) - (seq_t)1) / 9) + (seq_t)6;
        break;
    default:
        res = (seq_t)0; /* unreachable */
    }
    return res;
}
