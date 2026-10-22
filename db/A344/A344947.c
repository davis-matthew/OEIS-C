/*
 * A344947  Number of open tours by a biased rook on a specific A070941(n) X 1
 *          board, which ends on a black cell, where cells are colored white
 *          or black according to the binary representation of 2n.
 * https://oeis.org/A344947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A284005(2*A053645(n))
 * Range:     u8 n <= 28, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation is performed directly by enumerating all
   Hamiltonian paths of a biased rook on the board derived
   from the binary representation of 2 n. */

/* Platform requirements:
   idx_t and seq_t are available from the build.
   The function has external linkage and returns a seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MAX_L 15                     /* maximum board length (bit‐width of 2*n) */
#define MAX_MASK (1U << MAX_L)       /* 2^15 = 32768 */

/* Global storage for DP values and a stamp to avoid repeated clearing. */
static seq_t dp[MAX_MASK][MAX_L];
static uint32_t stamp[MAX_MASK][MAX_L];
static uint32_t cur_stamp = 1U;

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 even though the board has no black cell. */
    if (n == 0)
        return (seq_t)1;

    /* Compute 2*n and its bit‑length. */
    idx_t two_n = 2 * n;
    idx_t L = 0;
    idx_t temp = two_n;
    while (temp > 0) {
        ++L;
        temp >>= 1;
    }

    /* Board colors: bit i corresponds to index i (LSB → leftmost cell). */
    unsigned char colors[MAX_L];
    for (idx_t i = 0; i < L; ++i)
        colors[i] = (unsigned char)((two_n >> i) & 1U);

    idx_t fullmask = (1U << L) - 1U;
    idx_t curr = cur_stamp;
    if (curr == 0) {          /* wrap‑around protection */
        memset(stamp, 0, sizeof(stamp));
        curr = 1U;
    }
    cur_stamp = curr + 1U;

    /* Initialise starting positions. */
    for (idx_t i = 0; i < L; ++i) {
        idx_t mask = 1U << i;
        dp[mask][i] = (seq_t)1;
        stamp[mask][i] = curr;
    }

    /* Dynamic programming over subsets. */
    for (idx_t mask = 1U; mask <= fullmask; ++mask) {
        for (idx_t pos = 0; pos < L; ++pos) {
            if (stamp[mask][pos] != curr)
                continue;   /* state not reached in this call */

            seq_t cur_val = dp[mask][pos];
            if (mask == fullmask)
                continue;   /* already a complete tour */

            for (idx_t j = 0; j < L; ++j) {
                if (mask & (1U << j))
                    continue;          /* already visited */

                if (colors[pos] == 0 && j <= pos)
                    continue;          /* white cells can move only left (higher index) */

                idx_t nextmask = mask | (1U << j);
                if (stamp[nextmask][j] != curr) {
                    dp[nextmask][j] = (seq_t)0;
                    stamp[nextmask][j] = curr;
                }
                dp[nextmask][j] += cur_val;
            }
        }
    }

    /* Sum all complete tours that end on a black cell. */
    seq_t result = (seq_t)0;
    for (idx_t pos = 0; pos < L; ++pos) {
        if (colors[pos] == 1 && stamp[fullmask][pos] == curr)
            result += dp[fullmask][pos];
    }

    return result;
}
