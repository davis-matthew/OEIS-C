/*
 * A358148  Aliquot sequence starting at 326.
 * https://oeis.org/A358148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 12 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A358148: aliquot sequence starting at 326
   Recurrence: a(n+1) = A001065(a(n))  (sum of proper divisors of a(n))
   Initial value: a(0) = 326                */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */

/* Compute the aliquot sum of a value.
   The result fits in the same type as the argument. */
static seq_t aliquot_sum(seq_t n)
{
    /* 0 and 1 have no proper divisors */
    if (n <= (seq_t)1)
        return (seq_t)0;

    seq_t sum = (seq_t)1;   /* 1 is always a proper divisor of n > 1 */
    seq_t i   = (seq_t)2;

    /* Loop while i <= n/i to avoid overflow on i*i */
    while (i <= n / i) {
        if (n % i == (seq_t)0) {
            sum += i;
            seq_t other = n / i;
            if (other != i)   /* avoid double‑counting a perfect square */
                sum += other;
        }
        ++i;
    }
    return sum;
}

/* Main sequence function.  Computes a(n) by iterating from a(0). */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)326;    /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        cur = aliquot_sum(cur);
        if (cur == (seq_t)0)   /* once we hit 0, all further terms are 0 */
            break;
    }
    return cur;
}
