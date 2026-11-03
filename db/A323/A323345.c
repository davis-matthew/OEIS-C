/*
 * A323345  Square array read by ascending antidiagonals: T(n, k) is the
 *          number of partitions of n where parts, if sorted in ascending
 *          order, form an arithmetic progression (AP) with common difference
 *          of k; n >= 1, k >= 0.
 * https://oeis.org/A323345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A323345
 * a(n) is obtained by reading the triangle T(n,k) (n>=1,k>=0) in ascending antidiagonals.
 * Each T(n,k) counts partitions of n into an arithmetic progression with common difference k.
 * T(n,k) = # of m>=1 such that n - k*m*(m-1)/2 > 0 and divisible by m.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Map the sequence index to the (n,k) pair in the triangle */
static void pair_from_index(idx_t pos, idx_t *n_out, idx_t *k_out)
{
    /* Find smallest d such that d*(d+1)/2 >= pos */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < pos) high <<= 1;

    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= pos)
            high = mid;
        else
            low = mid;
    }
    idx_t d = high;                /* diagonal number (starting from 1) */

    idx_t prev = (d - 1) * d / 2;   /* number of elements before this diagonal */
    idx_t offset = pos - prev - 1;  /* 0-based offset inside the diagonal */

    *k_out = offset;               /* column index (k) */
    *n_out = d - offset;           /* row index (n) */
}

/* Helper to count T(n,k) */
static seq_t count_T(idx_t n, idx_t k)
{
    seq_t cnt = 0;
    long long N = (long long)n;       /* small enough to fit in signed 64-bit */
    long long K = (long long)k;

    if (K == 0) {
        /* Classic divisor count: m must divide n, 1 <= m <= n */
        for (long long m = 1; m <= N; ++m) {
            if (N % m == 0) ++cnt;
        }
    } else {
        for (long long m = 1; ; ++m) {
            unsigned __int128 prod = (unsigned __int128)K * m * (m - 1) / 2;
            if (prod >= (unsigned __int128)N) break;   /* rem <= 0, not valid */
            long long rem = N - (long long)prod;        /* rem > 0 */
            if (rem % m == 0) ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t N, K;
    pair_from_index(n, &N, &K);
    return count_T(N, K);
}
