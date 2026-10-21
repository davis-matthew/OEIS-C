/*
 * A265411  a(0) = 1, a(1) = 7, otherwise, if A240025(n-1) = 1 [when n is in
 *          A033638] a(n) = 3, otherwise a(n) = 1.
 * https://oeis.org/A265411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 7; for n ≥ 2, a(n)=3 if n is in A033638,
 *            otherwise a(n)=1. We generate A033638 by the recurrence t_1=2,
 *            t_j = t_{j-1} + floor((j+1)/2) for j ≥ 2
 * Range:     u8 n <= 9896, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   in_A033638
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/in_A033638.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    return in_A033638(n) ? (seq_t)3 : (seq_t)1;
}
