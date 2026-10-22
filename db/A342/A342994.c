/*
 * A342994  a(n) = (1000^n - 1)*(220/333).
 * https://oeis.org/A342994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (1000^n - 1)*(220/333) Equivalent recurrence: a(1) = 660
 *            a(2) = 660660 a(n) = 1001*a(n-1) - 1000*a(n-2) for n ≥ 3 This
 *            implementation evaluates the recurrence iteratively using only
 *            two temporary variables, so it works for any seq_t type
 *            (built‑in integer or arbitrary‑precision)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, safe guard */
    if (n == 1) return (seq_t)660;
    if (n == 2) return (seq_t)660660;

    seq_t prev2 = (seq_t)660;            /* a(1) */
    seq_t prev1 = (seq_t)660660;         /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* cur = 1001*prev1 - 1000*prev2 */
        cur = prev1 * (seq_t)1001 - prev2 * (seq_t)1000;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
