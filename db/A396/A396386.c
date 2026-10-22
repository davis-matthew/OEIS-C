/*
 * A396386  a(n) = r(n,n), where r(1,k)=f(k)=round(k*c), r(n,k)=f(r(n-1,k)),
 *          c=3/2, using banker's rounding.
 * https://oeis.org/A396386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f^n(n), where f(k) = round(k*3/2) using banker's rounding
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   round_k3h2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence A396386. */

#include <stdbool.h>
#include "../utils/round_k3h2.h"

seq_t a(idx_t n)
{
    seq_t cur = n;
    for (idx_t i = 0; i < n; ++i)
        cur = round_k3h2(cur);
    return cur;
}
