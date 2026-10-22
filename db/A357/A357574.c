/*
 * A357574  a(n) is the maximum number of pairs that sum to a power of 2 in a
 *          set of n consecutive odd numbers.
 * https://oeis.org/A357574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes maximum number of pairs whose sum is a power of 2 in a set
   of n consecutive odd numbers. The optimal set is the one described
   in OEIS A357574: n−x negative odds followed by x positive odds,
   where x is chosen to maximize the number of such pairs. The algorithm
   tests all possible x (0 … n) and counts the pairs for each case
   in O(n log n) time. */
#include <stdint.h>

static inline seq_t count_pairs(idx_t n, long long t)
{
    /* number of unordered pairs i<j in 0…n−1 with i+j = t */
    if (t <= 0) return 0;
    long long maxsum = 2 * (long long)n - 2;
    if (t > maxsum) return 0;

    /* compute lower and upper bounds for i */
    idx_t low  = (t > (long long)(n - 1)) ? (idx_t)(t - (long long)(n - 1)) : 0;
    idx_t high = (idx_t)((t - 1) / 2);          /* floor((t‑1)/2) */

    if (high < low) return 0;
    return (seq_t)(high - low + 1);
}

seq_t a(idx_t n)
{
    seq_t best = 0;
    idx_t maxIdxSum = 2 * n - 2;          /* maximum value of i + j */

    for (idx_t x = 0; x <= n; ++x) {
        idx_t neg = n - x;
        long long s = -(2 * (long long)neg) + 1;           /* start odd number */

        long long up = s + (long long)maxIdxSum;           /* highest 2^(k‑1) to test */
        if (up < 1) continue;                             /* no positive powers of two */

        seq_t cnt = 0;
        idx_t p = 1;                                      /* p = 2^(k‑1) */
        while (p <= (idx_t)up) {
            long long t = (long long)p - s;
            if (t > 0 && t <= (long long)maxIdxSum)
                cnt += count_pairs(n, t);
            /* no need to limit shift: maximum up is < 2*10000, fits in 64‑bit */
            p <<= 1;
        }
        if (cnt > best) best = cnt;
    }
    return best;
}
