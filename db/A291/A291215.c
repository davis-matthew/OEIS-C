/*
 * A291215  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 7.
 * https://oeis.org/A291215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(3k-2) = 7*(10^(22k)-1)/69, * a(3k-1) = 8*(10^(22k)-1)/69, *
 *            a(3k) = 9*(10^(22k)-1)/69. * with k = (n+2)/3 if n%3==1, k =
 *            (n+1)/3 if n%3==2, k = n/3 otherwise
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 3, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1, but guard against zero */

    idx_t mod = n % 3;
    idx_t k;
    int factor;

    if (mod == 1) {          /* n = 3k-2 */
        k = (n + 2) / 3;
        factor = 7;
    } else if (mod == 2) {   /* n = 3k-1 */
        k = (n + 1) / 3;
        factor = 8;
    } else {                 /* n = 3k   */
        k = n / 3;
        factor = 9;
    }

    seq_t pw = pow10(22 * k);        /* 10^(22k) */
    seq_t val = pw - 1;              /* 10^(22k) - 1   */
    val *= (seq_t)factor;            /* factor*(10^(22k)-1) */
    val /= 69;                       /* exact division by 69 */

    return val;
}
