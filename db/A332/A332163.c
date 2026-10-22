/*
 * A332163  a(n) = 6*(10^(2*n+1)-1)/9 - 3*10^n.
 * https://oeis.org/A332163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 - 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t1   = pow10(2*n + 1);
    seq_t sub  = t1 - (seq_t)1;
    seq_t rep  = sub / (seq_t)9;
    seq_t part = rep * (seq_t)6;
    seq_t part2 = pow10(n) * (seq_t)3;
    return part - part2;
}
