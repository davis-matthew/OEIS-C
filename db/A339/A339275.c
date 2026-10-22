/*
 * A339275  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists the terms of A040000: 1, 2, 2, 2, ... interleaved
 *          with k-1 zeros, and the first element of column k is in row
 *          k(k+1)/2.
 * https://oeis.org/A339275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339275: Irregular triangle read by rows:
 * a(n) is the n-th entry in the triangular array where
 * column k contains the terms of A040000 = 1,2,2,2,… interleaved
 * with k−1 zeros, and the first element of column k occurs in
 * row k(k+1)/2.  The function iterates rows and columns until it
 * reaches the desired position; it therefore works for arbitrary n
 * (within machine limits). */

#include <stdint.h>

/* Integer square‑root for 64‑bit values, implemented locally to avoid
 * external dependencies.  Returns ⌊√v⌋ for a 128‑bit input. */
static idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0, hi = (idx_t)1 << 32; /* √(2^128) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the n-th term of A339275. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, but return 0 */
        return (seq_t)0;

    idx_t cumulative = 0;  /* number of terms already produced */

    for (idx_t r = 1; ; ++r) {
        /* Number of columns active in this row. */
        unsigned __int128 tmp = (unsigned __int128)8 * r + 1;
        idx_t kmax = (isqrt_u128(tmp) - 1) / 2;

        for (idx_t k = 1; k <= kmax; ++k) {
            /* start row of column k: k(k+1)/2 */
            unsigned __int128 prod = (unsigned __int128)k * (k + 1);
            idx_t start = (idx_t)(prod / 2);

            idx_t diff = r - start;           /* r >= start by construction */
            if ((diff % k) == 0) {           /* a term from A040000 */
                idx_t t = diff / k + 1;       /* term number in A040000 */
                seq_t val = (t == 1) ? (seq_t)1 : (seq_t)2;
                ++cumulative;
                if (cumulative == n)
                    return val;
            } else {                         /* a zero */
                ++cumulative;
                if (cumulative == n)
                    return (seq_t)0;
            }
        }
    }
}
