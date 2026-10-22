/*
 * A376182  Triangle T read by rows: T(n, k) = (2*n^2 + 4*n + 1 - (-1)^n) / 4
 *          - (1 + (-1)^k) * (n - k) - k.
 * https://oeis.org/A376182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(N,K) where N is the smallest integer with N*(N+1)/2 >=
 *            n and K = n - N*(N-1)/2. T(N,K) = (2*N^2 + 4*N + 1 - (-1)^N)/4 -
 *            (1 + (-1)^K)*(N-K) - K Implemented using only integer arithmetic
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row N such that N*(N+1)/2 >= n using binary search. */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t N = low;

    /* Compute K = n - (N-1)*N/2. */
    idx_t prev = (idx_t)(((unsigned __int128)(N - 1) * N) / 2);
    idx_t K = n - prev;

    /* Compute the base term using a wider type to avoid overflow. */
    unsigned __int128 base;
    if ((N & 1ULL) == 0) {
        /* N is even: use N*(N+2)/2 */
        base = (unsigned __int128)N * (N + 2) / 2;
    } else {
        /* N is odd: use (N+1)^2/2 */
        base = (unsigned __int128)(N + 1) * (N + 1) / 2;
    }

    /* Adjust for even K. */
    if ((K & 1ULL) == 0) {
        base -= (unsigned __int128)2 * (N - K);
    }
    /* Subtract K. */
    base -= (unsigned __int128)K;

    return (seq_t)base;
}
