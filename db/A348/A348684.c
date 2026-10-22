/*
 * A348684  Triangle read by rows, T(n, k) = 2*n - HammingWeight(k), for 0 <=
 *          k <= n.
 * https://oeis.org/A348684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r - popcount(k) * where r = floor((sqrt(8*n+1)-1)/2)
 *            (row index) * and k = n - r*(r+1)/2 (column index) * The
 *            triangle T(n,k) = 2*n - popcount(k) is read by rows * (row 0,
 *            then row 1, …)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute popcount of a 64‑bit integer. */
static unsigned popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (1) {
        unsigned __int128 next_start = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (next_start > n)
            break;
        r++;
    }

    /* Compute the column index k. */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    /* Compute the value: 2*r - popcount(k). */
    unsigned int pc = popcount_u64((uint64_t)k);
    seq_t val = (seq_t)((seq_t)2 * r - pc);
    return val;
}
