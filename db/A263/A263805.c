/*
 * A263805  Binary representation of the n-th iteration of the "Rule 157"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A263805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n = 0) a(n) = (10*10^n*(10^n+1)/11 - 1)/9 for odd n >
 *            0 a(n) = (10^n*(10^n+10)/11 - 1)/9 for even n > 0 (computed
 *            using 10^n = pow10(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t x = pow10(n);          /* 10^n  */
    seq_t num;

    if (n & (idx_t)1) {          /* odd n  */
        seq_t tmp = x + (seq_t)1;   /* divisible by 11 */
        seq_t div = tmp / (seq_t)11;
        num = x * div;              /* x * ((x+1)/11) */
        num = num * (seq_t)10;      /* *10 */
    } else {                     /* even n  */
        seq_t tmp = x + (seq_t)10;  /* divisible by 11 */
        seq_t div = tmp / (seq_t)11;
        num = x * div;              /* x * ((x+10)/11) */
    }

    seq_t res = (num - (seq_t)1) / (seq_t)9;
    return res;
}
