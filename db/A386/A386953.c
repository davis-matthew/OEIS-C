/*
 * A386953  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 9.
 * https://oeis.org/A386953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 590, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_not_div_by_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{r=0..n}  #{k : 0 ≤ k ≤ r  and  C(r,k) not divisible by 9}
 * 
 * The implementation uses Kummer’s theorem: the exponent of 3 in C(r,k) equals
 * the number of borrows when subtracting k from r in base 3.  We count all
 * k for which at most one borrow occurs, which guarantees the exponent
 * is 0 or 1, i.e. C(r,k) is not divisible by 9.  The algorithm is O(n·log₃n)
 * and works for any n up to the limits described in the problem statement.
 */

#include <stdint.h>
#include "../utils/count_not_div_by_9.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t r = 0; r <= n; ++r) {
        total += count_not_div_by_9(r);
    }
    return total;
}
