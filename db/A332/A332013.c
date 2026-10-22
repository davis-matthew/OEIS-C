/*
 * A332013  T(n, k) is the least positive m such that floor(n/m) AND
 *          floor(k/m) = 0 (where AND denotes the bitwise AND operator).
 *          Square array T(n, k) read by antidiagonals, n >= 0 and k >= 0.
 * https://oeis.org/A332013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9812, u16 n <= 9812, u32 n <= 9812, u64 n <= 9812,
 *            u128 n <= 9812, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min_{m>0} { m | floor(i/m) & floor(j/m) == 0 },
 * where i and j are the antidiagonal coordinates of n
 * (i+j = floor((sqrt(8n+1)-1)/2), i = n - j*(j+1)/2).
 */

#include <stdint.h>

static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

static seq_t T_ij(idx_t i, idx_t j)
{
    /* special cases: simple shortcuts */
    if ((i & j) == 0)
        return (seq_t)1;          /* T(n,k)=1 iff n AND k = 0           */
    if (i == j)
        return (seq_t)(i + 1);    /* T(n,n)=n+1                          */

    seq_t Si = (seq_t)i;
    seq_t Sj = (seq_t)j;
    idx_t upper = (i > j ? i : j) + 1;

    for (idx_t m = 1; m <= upper; ++m) {
        seq_t ai = Si / (seq_t)m;
        seq_t aj = Sj / (seq_t)m;
        if ((ai & aj) == 0)
            return (seq_t)m;
    }
    /* should never reach here */
    return (seq_t)(upper + 1);
}

seq_t a(idx_t n)
{
    /* compute antidiagonal coordinates (i,j) for index n */
    unsigned __int128 u = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_u128(u) - 1) >> 1;          /* r = floor((sqrt(8n+1)-1)/2) */

    unsigned __int128 prefix = (unsigned __int128)r * (r + 1) / 2;
    idx_t i = n - (idx_t)prefix;
    idx_t j = r - i;

    return T_ij(i, j);
}
