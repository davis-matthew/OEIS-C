/*
 * A271705  Triangle read by rows, T(n,k) = Sum_{j=0..n}
 *          (-1)^(n-j)*C(-j-1,-n-1)*L(j,k), L the unsigned Lah numbers
 *          A271703, for n>=0 and 0<=k<=n.
 * https://oeis.org/A271705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r, k) where * r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * T(r, k) is defined by T(0,0)=1, * T(n,0)=1, T(n,k)=0
 *            for n<k, * and T(n,k) = (n/k)*T(n-1,k-1) + n*T(n-1,k)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 79, u64 n <= 211,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned 64-bit numbers */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1<<32) - 1; /* sqrt(2^64-1) < 2^32 */
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute row r and column k from flattened index n */
    idx_t D = n * 8u + 1u;
    idx_t sqrtD = _floor_isqrt_u64(D);
    idx_t r = (sqrtD - 1u) / 2u;
    /* prefix number of elements before row r */
    unsigned __int128 pref128 = (unsigned __int128)r * (r + 1u) / 2u;
    idx_t pref = (idx_t)pref128;          /* fits in idx_t */
    idx_t k = n - pref;

    /* Allocate two auxiliary rows of length r+1 */
    size_t len = (size_t)(r + 1u);
    seq_t *prev = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!prev || !cur) {
        /* In case of allocation failure, return 0 (unlikely) */
        return (seq_t)0;
    }

    /* Initialize row 0 */
    prev[0] = (seq_t)1;
    /* If r == 0, answer is prev[0] */
    if (r > 0) {
        for (idx_t i = 1; i <= r; ++i) {
            /* cur[0] and cur[i] are always 1 */
            cur[0] = (seq_t)1;
            cur[i] = (seq_t)1;
            for (idx_t j = 1; j < i; ++j) {
                /* T(i,j) = (i/j)*T(i-1,j-1) + i*T(i-1,j) */
                seq_t term1 = (seq_t)i * prev[j];
                seq_t term2 = ((seq_t)i * prev[j-1]) / (seq_t)j;
                cur[j] = term1 + term2;
            }
            /* Swap prev and cur for next iteration */
            seq_t *tmp = prev;
            prev = cur;
            cur = tmp;
        }
    }

    seq_t result = prev[k];

    free(prev);
    free(cur);
    return result;
}
