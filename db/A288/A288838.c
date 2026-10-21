/*
 * A288838  a(n) = (1/4!)*3^(n+2)*(n+7)*(n+2)*(n+1)*(n).
 * https://oeis.org/A288838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*3^(n+2)*(n+7)*(n+2)*(n+1)*n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow3_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3_n.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;   /* offset is 1; a(0) undefined, return 0 */
    }
    seq_t prod = (seq_t)n * ((seq_t)n + 1);
    prod *= ((seq_t)n + 2);
    prod *= ((seq_t)n + 7);
    /* 24 = 8 * 3; divide by 8 first */
    seq_t q = prod / (seq_t)8;
    seq_t pow = pow3_n((idx_t)(n + 1));   /* 3^(n+1) */
    return q * pow;
}
