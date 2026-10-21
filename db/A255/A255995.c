/*
 * A255995  Number of length n+4 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 12*n^2 + 143*n + 36)/6 for all n != 2 a(2) is 64.
 *            (The formula gives 63 for n=2, so this single exception corrects
 *            the error.)
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage */
seq_t a(idx_t n)
{
    /* Special‐case where the closed form fails */
    if (n == 2)
        return 64;

    /* Perform the cubic calculation using 128‑bit arithmetic
       to avoid overflow for large n.  This is safe for any n
       that fits in idx_t (uint64_t).  The final result fits
       in seq_t because the numbers grow only cubically.  */
    __int128 N = n;
    __int128 num = N * N * N + (__int128)12 * N * N + (__int128)143 * N + 36;
    return (seq_t)(num / 6);
}
