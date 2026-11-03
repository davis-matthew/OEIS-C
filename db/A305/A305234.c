/*
 * A305234  Number of successors of n having the same value of A001221 as n.
 * https://oeis.org/A305234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A305234: for n ≥ 1 count the number of successive integers > n
   that have the same number of distinct prime divisors as n 
   (i.e. the same value of A001221). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    int w = omega((uint64_t)n);
    seq_t cnt = 0;

    for (idx_t m = n + 1; ; ++m)
    {
        if (omega((uint64_t)m) != w)
            break;
        ++cnt;
    }

    return cnt;
}
