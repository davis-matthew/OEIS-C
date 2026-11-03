/*
 * A354977  Triangle read by rows. T(n, k) =
 *          Sum_{j=0..n}((-1)^(n-j)*binomial(n, j)*j^(n+k)) / n!.
 * https://oeis.org/A354977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, and T(r,c) = Stirling2(r+c,r)
 * Range:     u8 n <= 7, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Determine the row index r and column offset c for position n. */
    seq_t tmp_val = (seq_t)8 * (seq_t)n + (seq_t)1;
    seq_t tmp = floor_sqrt(tmp_val);
    idx_t r = ((idx_t)tmp - (idx_t)1) / 2;

    idx_t cumulativePrev = r * (r + 1) / 2;
    idx_t offset = n - cumulativePrev;
    idx_t m = r + offset;   /* m = r + c = (row + column) */

    /* Compute Stirling numbers of the second kind S(m, r) via DP. */
    seq_t *s = mem_new(seq_t, (m + 1));
    if (!s) return (seq_t)0;
    for (idx_t i = 0; i <= m; ++i) s[i] = (seq_t)0;
    s[0] = (seq_t)1;   /* S(0,0)=1 */

    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = i; j >= 1; --j) {
            seq_t prod = mul_small(s[j], (unsigned int)j);
            s[j] = prod + s[j - 1];
        }
        s[0] = (seq_t)0;   /* S(i,0)=0 for i>0 */
    }

    seq_t res = s[r];
    mem_free(s);
    return res;
}
