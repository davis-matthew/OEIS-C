/*
 * A395330  Symmetric array read by antidiagonals: A(n,k) = tau(n*k), where
 *          tau = A000005.
 * https://oeis.org/A395330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = τ(i*j), where (i,j) is the position of n on the
 *            antidiagonal of the symmetric array A(i,j) = τ(i*j). The first
 *            element a(1) corresponds to i=j=1
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Locate the diagonal d containing the n‑th element */
    idx_t low  = 1;
    idx_t high = 1;

    /* Increase high until the triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high *= 2;

    /* Binary search for smallest d with d*(d+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t d = low;

    /* Number of elements before this diagonal */
    unsigned __int128 prev = (unsigned __int128)(d - 1) * d / 2;
    /* Zero‑based offset within diagonal */
    idx_t offset = (idx_t)((unsigned __int128)n - prev - 1);

    /* Coordinates on the diagonal */
    idx_t i = offset + 1;        /* increases */
    idx_t j = d - offset;        /* decreases */

    /* Compute τ(i*j) using the supplied divisor count routine */
    uint64_t product = (uint64_t)i * (uint64_t)j;
    return (seq_t)divisor_count_u64(product);
}
