/*
 * A261424  Difference between n and the largest palindrome <= n.
 * https://oeis.org/A261424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 3426, u16 n <= 8825, u32 n <= 8825, u64 n <= 8825,
 *            u128 n <= 8825, big any n
 * Helpers:   largest_pal_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261424  
   Formula: a(n) = n - largest palindrome ≤ n
   (largest palindrome is constructed from the decimal digits of n) */

#include <stdbool.h>
#include "../utils/largest_pal_leq.h"

/* External linkage function required by the problem statement. */
seq_t a(idx_t n)
{
    seq_t pal = largest_pal_leq(n);
    return (seq_t)n - pal;
}
