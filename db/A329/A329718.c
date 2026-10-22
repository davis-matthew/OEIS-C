/*
 * A329718  The number of open tours by a biased rook on a specific f(n) X 1
 *          board, where f(n) = A070941(n) and cells are colored white or
 *          black according to the binary representation of 2n.
 * https://oeis.org/A329718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Hamiltonian paths on a 1 × f(n) board where
 *            f(n) = bit length of 2n and each cell is colored according to
 *            the binary digits of 2n (0 → white, 1 → black). A rook on a
 *            white cell may move only to the left and a rook on a black cell
 *            may move only to the right. The function counts al...
 * Range:     u8 n <= 39, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline int bitlen_uint64(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 1;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 1. */
    if (n == 0) return (seq_t)1;

    /* Length of board (f(n)) = number of bits of 2n. */
    int m = bitlen_uint64((uint64_t)n) + 1;   /* bitlen(2n) = bitlen(n)+1 for n>0 */

    /* Color of each cell:
         0 → white (can move only left),
         1 → black (can move only right),
       according to binary digits of 2n from most to least significant. */
    unsigned char color[64];  /* maximum size needed for n up to 10 000 */
    for (int i = 0; i < m; ++i) {
        int pos = m - 1 - i;          /* bit position in 2n (0 = LSB) */
        if (pos == 0) {               /* LSB of 2n is always 0 (even number) */
            color[i] = 0;              /* white */
        } else {
            color[i] = (unsigned char)((n >> (pos - 1)) & 1ULL);
        }
    }

    /* DP over subsets: dp[mask * m + last] = number of tours that
       visit cells in 'mask' and end at cell 'last'. */
    size_t dp_size = ((size_t)1 << m) * (size_t)m;
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure, return 0 */

    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;

    /* Initialize with paths of length 1 (start at each cell). */
    for (int i = 0; i < m; ++i) {
        size_t idx = ((size_t)1 << i) * (size_t)m + (size_t)i;
        dp[idx] = (seq_t)1;
    }

    uint64_t full_mask = ((uint64_t)1 << m) - 1;
    /* Iterate over all masks. */
    for (uint64_t mask = 1; mask <= full_mask; ++mask) {
        for (int last = 0; last < m; ++last) {
            if (!(mask & ((uint64_t)1 << last))) continue;
            seq_t ways = dp[mask * (size_t)m + (size_t)last];
            if (ways == (seq_t)0) continue;
            for (int nxt = 0; nxt < m; ++nxt) {
                if (mask & ((uint64_t)1 << nxt)) continue;
                /* Movement rule based on color of current cell. */
                if ((color[last] == 1 && nxt > last) ||
                    (color[last] == 0 && nxt < last)) {
                    uint64_t newmask = mask | ((uint64_t)1 << nxt);
                    dp[newmask * (size_t)m + (size_t)nxt] += ways;
                }
            }
        }
    }

    /* Sum over all Hamiltonian paths that visit every cell. */
    seq_t result = (seq_t)0;
    for (int last = 0; last < m; ++last) {
        result += dp[full_mask * (size_t)m + (size_t)last];
    }

    free(dp);
    return result;
}
