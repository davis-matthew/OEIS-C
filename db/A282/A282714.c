/*
 * A282714  Base-2 generalized Pascal triangle P_2 read by rows (see Comments
 *          for precise definition).
 * https://oeis.org/A282714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the number of occurrences of the binary string of index
 *            p (within the n‑th row) as a scattered substring of the binary
 *            string of index r, where r is the row containing n. The value is
 *            computed by counting subsequences
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
/* The following prototype is provided by the build system.
   It returns ⌊√u⌋ for an unsigned 64‑bit integer. */
extern idx_t _floor_isqrt_u64(idx_t u);
 
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2   */
    idx_t r = 0;
    while (1) {
        idx_t next = r + 1;
        /* use 128‑bit arithmetic to avoid overflow of the product */
        __uint128_t val = (__uint128_t)next * (next + 1) / 2;
        if (val > n) break;
        r = next;
    }
    idx_t start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t pos   = n - start;                /* column inside the row */
 
    /* Length of binary representations (most significant bit first) */
    int lenS = (r == 0) ? 0 : (int)(64 - __builtin_clzll(r));
    int lenT = (pos == 0) ? 0 : (int)(64 - __builtin_clzll(pos));
 
    /* Empty target string occurs once */
    if (lenT == 0) return (seq_t)1;
    /* Longer target than source cannot match */
    if (lenT > lenS) return (seq_t)0;
 
    /* 1‑D DP array for counting subsequences */
    seq_t dp[65];                         /* lenT ≤ 64, so array fits */
    for (int i = 0; i <= lenT; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;
 
    /* Process each bit of source string (most significant bit first) */
    for (int j = 0; j < lenS; ++j) {
        int src_bit = (int)((r >> (lenS - 1 - j)) & 1);
        /* Update dp from the end to avoid overwriting needed values */
        for (int i = lenT; i >= 1; --i) {
            int tgt_bit = (int)((pos >> (lenT - i)) & 1);
            if (src_bit == tgt_bit) dp[i] = dp[i] + dp[i - 1];
        }
    }
 
    return dp[lenT];
}
