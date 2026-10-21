/*
 * A253438  Number of (4+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 109 for n=1, 108 for n=2, 121 for n=3, 84 + 9·2^(n‑1) for
 *            n≥4 * This follows the recurrence a(n) = 3·a(n‑1) – 2·a(n‑2) for
 *            n>5, * and the closed‑form a(n) = 84 + 9·2^(n‑1) for n>3. The
 *            implementation * uses only the allowed arithmetic operators and
 *            works for any integer * type supplied by...
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the n‑th term of sequence A253438.  */
seq_t
a(idx_t n)
{
    /* Handle the first three special cases. */
    if (n == (idx_t)1)
        return (seq_t)109;
    if (n == (idx_t)2)
        return (seq_t)108;
    if (n == (idx_t)3)
        return (seq_t)121;

    /* For n ≥ 4 compute 84 + 9·2^(n‑1).  */
    seq_t pow = (seq_t)1;            /* pow = 2^(n‑1) */
    for (idx_t i = (idx_t)1; i < n; ++i)
        pow *= (seq_t)2;             /* multiply by 2 repeatedly */

    return ((seq_t)9 * pow) + ((seq_t)84);
}
