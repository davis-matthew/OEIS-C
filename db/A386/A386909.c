/*
 * A386909  Iterates of g(x), starting at x = 0 until g(x) == 2 (mod 3), where
 *          g(x) = (5*x + 18)/3 if x == 0 (mod 3) and g(x) = (5*x + 22)/3 if x
 *          == 1 (mod 3).
 * https://oeis.org/A386909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(0)=0; for n>0, a(n)=g(a(n‑1)), where g(x) = (5*x + 18)/3 if x
 *            ≡ 0 (mod 3), (5*x + 22)/3 if x ≡ 1 (mod 3). The sequence
 *            terminates when a(n) ≡ 2 (mod 3)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0u)
        return (seq_t)0u;

    seq_t x = (seq_t)0u;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t next;
        if (x % 3u == 0u)
            next = (5u * x + 18u) / 3u;
        else
            next = (5u * x + 22u) / 3u;
        x = next;

        /* If we have produced the requested term, return it */
        if (i == n)
            return x;

        /* Once a term ≡ 2 (mod 3) is produced, the sequence stops */
        if (x % 3u == 2u)
            break;
    }

    /* The requested index lies beyond the terminating term.  The OEIS
       sequence is finite; undefined terms are reported as 0. */
    return (seq_t)0u;
}
