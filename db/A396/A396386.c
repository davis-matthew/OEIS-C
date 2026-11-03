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
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence A396386. */

#include <stdbool.h>

static seq_t round_k3h2(seq_t k)
{
    seq_t three_k = k * 3;
    if ((three_k & 1) == 0)          /* 3*k is even -> exact division */
        return three_k / 2;
    /* 3*k is odd: 3*k = 2*q + 1 => q = (3*k-1)/2 */
    seq_t q = (three_k - 1) / 2;
    if ((q & 1) == 0)                /* q even -> choose q */
        return q;
    return q + 1;                    /* q odd -> choose q+1 (even) */
}

seq_t a(idx_t n)
{
    seq_t cur = n;
    for (idx_t i = 0; i < n; ++i)
        cur = round_k3h2(cur);
    return cur;
}
