/*
 * A361682  Array read by descending antidiagonals. A(n, k) is the number of
 *          multiset combinations of {0, 1} whose type is defined in the
 *          comments. Also A(n, k) = hypergeom([-k, -2], [1], n).
 * https://oeis.org/A361682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A361682 (Array read by descending antidiagonals)
 *  a(n) = A( n_row, n_col )
 *  where (n_row,n_col) are derived from n by traversing
 *  the array along descending antidiagonals.
 *  A(n_row, n_col) = 1 + n_row*n_col*(4 + n_row*(n_col-1))/2
 *
 *  The enumeration order is:
 *    for s = 0,1,2,...
 *        for i = 0 .. s
 *            j = s - i
 *            output A(i,j)
 *
 *  This function computes a(n) for any n >= 0.
 */

#include <stdint.h>

/* Find the largest t such that t*(t+1)/2 <= idx.
   All arithmetic is done in 128‑bit to avoid overflow. */
static uint64_t find_t(uint64_t idx)
{
    uint64_t low  = 0;
    uint64_t high = 1;               /* start with 1 */

    /* Expand upper bound until it exceeds idx. */
    while ((unsigned __int128)high * (high + 1) / 2 <= idx) {
        high <<= 1;
    }

    /* Binary search between low and high. */
    while (low + 1 < high) {
        uint64_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= idx)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    uint64_t idx = n;                /* index */
    uint64_t t   = find_t(idx);      /* sum i+j for this diagonal */

    uint64_t tri = (unsigned __int128)t * (t + 1) / 2; /* elements before this diagonal */
    uint64_t pos = idx - tri;                /* position inside diagonal */

    uint64_t i = pos;                /* row = n_row */
    uint64_t j = t - pos;            /* column = n_col */

    /* Compute A(i,j) = 1 + i*j*(4 + i*(j-1))/2 */
    seq_t N = (seq_t)i;
    seq_t K = (seq_t)j;

    seq_t prod1  = N * K;               /* i*j */

    seq_t Kminus1 = (K == 0) ? (seq_t)0 : K - 1;
    seq_t prod2  = N * Kminus1;         /* i*(j-1) */

    seq_t inner  = 4 + prod2;           /* 4 + i*(j-1) */

    seq_t prod   = prod1 * inner;       /* i*j*(4 + i*(j-1)) */
    seq_t half   = prod / 2;            /* divide by 2 */

    return half + 1;                    /* final value */
}
