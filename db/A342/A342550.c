/*
 * A342550  For n>=3, a(n) is the sum of the indices of n seen as an m-gonal
 *          number.
 * https://oeis.org/A342550
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A342550:
   For n ≥ 3, a(n) = Σ k, over all k ≥ 2 such that n is the k‑th m‑gonal
   number for some integer m ≥ 3.  The sum always contains k = 2
   (since P(m,2)=m).  The function uses an integer square‑root
   routine and avoids floating‑point or external helpers. */

#include <stdint.h>

static inline uint64_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;          /* hi² > 2⁶⁴   */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Offset 3 – behaviour for n < 3 is irrelevant. */
    if (n < 3) return 0;

    seq_t sum = 2;          /* k = 2 is always counted (m = n). */

    /* Upper bound for k: k(k+1)/2 ≤ n  →  k ≤ floor((√(8n+1)-1)/2). */
    uint64_t upper = floor_sqrt_u64(8ULL * n + 1ULL);
    uint64_t kmax  = (upper - 1ULL) / 2ULL;
    if (kmax < 3ULL) return sum;       /* no further k possible */

    for (uint64_t k = 3ULL; k <= kmax; ++k) {
        /* Compute: m = [2n + k(2k-4)] / [k(k-1)]   (integer arithmetic). */
        __int128 num   = (__int128)2 * n + (__int128)k * ( (__int128)2 * k - 4 );
        __int128 denom = (__int128)k * (k - 1);
        if (denom == 0) continue;   /* safety, never occurs (k ≥ 3). */
        if (num % denom != 0) continue;   /* m not integer */
        __int128 m = num / denom;
        if (m < 3) continue;                   /* m must be ≥ 3   */
        sum += k;
    }

    return sum;
}
