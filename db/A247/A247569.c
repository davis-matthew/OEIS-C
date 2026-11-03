/*
 * A247569  Base-8 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247569
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n ≥ 2 Let e = number of trailing zeros in binary expansion
 *            of n and m = n >> e (odd part). If m == 1 (n is a power of two):
 *            a(n) = n + 1 for n = 2,4 a(n) = 2*n - 7 for n ≥ 8 Else if n is
 *            odd: a(n) = n * ord_8(n) Else (n is even but not a power of
 *            two): a(n) = n * ord_8(m) + 1 where ord_8(k) i...
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t multiplicative_order_8(idx_t m)
{
    if (m <= 1)
        return 1;
    seq_t cur = (seq_t)8 % m;
    seq_t ord = 1;
    while (cur != 1) {
        cur = (cur * 8) % m;
        ++ord;
    }
    return ord;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* out of the defined range */

    /* Count trailing zeros (exponent of 2 in n). */
    idx_t e = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e;
    }
    idx_t odd_part = temp;       /* n >> e */

    /* Handle powers of two separately. */
    if (odd_part == 1) {
        if (e <= 2)              /* n = 2 or 4 */
            return (seq_t)n + 1;
        else                     /* n ≥ 8 */
            return (seq_t)(2 * n - 7);
    }

    /* n has odd part > 1. */
    seq_t ord = multiplicative_order_8(odd_part);
    if (n & 1)   /* n is odd */
        return (seq_t)n * ord;
    else         /* n is even, not a power of two */
        return (seq_t)n * ord + 1;
}
