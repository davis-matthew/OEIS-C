/*
 * A387815  a(n) = (1/3) * Sum_{k=0..n} binomial(6*n+3,6*k+1).
 * https://oeis.org/A387815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 8*64^n + 9*(-27)^n)/18, a(0)=1
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* compute 64^n and 27^n */
    seq_t pow64 = big_pow((seq_t)64, (seq_t)n);
    seq_t pow27 = big_pow((seq_t)27, (seq_t)n);
    
    /* 8*64^n and 9*27^n */
    seq_t term1 = pow64 * 8;
    seq_t term2 = pow27 * 9;
    
    /* numerator = 1 + term1 + (-1)^n * term2 */
    seq_t num = (seq_t)1 + term1;
    if ((n & 1ULL) == 0)  /* n even */
        num += term2;
    else                  /* n odd, subtract term2 */
        num -= term2;
    
    /* divide by 18 */
    return num / (seq_t)18;
}
