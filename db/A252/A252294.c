/*
 * A252294  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×3 arrays of digits 0..3 such that for
 *            every 3×3 subblock the sums of each row, each column, the main
 *            diagonal and the anti‑diagonal avoid the values {0,2,3,6,7}. The
 *            sequence is computed by a transfer‑matrix DP
 * Range:     u8 none, u16 n <= 10, u32 n <= 24, u64 n <= 52, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       Precomputation of digits, sums and the allowed‑transition
       matrix.  This is performed only once.                     */
    static uint64_t allowed_next[4096];
    static unsigned char row_digits[64][3];
    static unsigned char row_sum[64];
    static int initialized = 0;

    if (!initialized) {
        /* digits of each row (base‑4 representation) */
        for (int r = 0; r < 64; ++r) {
            row_digits[r][0] =  r & 3;
            row_digits[r][1] = (r >> 2) & 3;
            row_digits[r][2] = (r >> 4) & 3;
            row_sum[r] = row_digits[r][0] + row_digits[r][1] + row_digits[r][2];
        }

        /* forbidden sums : 0,2,3,6,7 */
        static const unsigned char forbid[10] = {1,0,1,1,0,0,1,1,0,0};

        for (int p = 0; p < 64; ++p) {
            for (int q = 0; q < 64; ++q) {
                uint64_t mask = 0;
                for (int r = 0; r < 64; ++r) {
                    int s0 = row_sum[p], s1 = row_sum[q], s2 = row_sum[r];
                    if (forbid[s0] || forbid[s1] || forbid[s2]) continue;
                    int c0 = row_digits[p][0] + row_digits[q][0] + row_digits[r][0];
                    int c1 = row_digits[p][1] + row_digits[q][1] + row_digits[r][1];
                    int c2 = row_digits[p][2] + row_digits[q][2] + row_digits[r][2];
                    if (forbid[c0] || forbid[c1] || forbid[c2]) continue;
                    int d  = row_digits[p][0] + row_digits[q][1] + row_digits[r][2];
                    if (forbid[d]) continue;
                    int ad = row_digits[p][2] + row_digits[q][1] + row_digits[r][0];
                    if (forbid[ad]) continue;
                    mask |= 1ULL << r;
                }
                allowed_next[(p << 6) | q] = mask;
            }
        }
        initialized = 1;
    }

    /* ------------------------------------------------------------
       DP arrays.  They are static to avoid allocating on the stack
       each call.  They are re‑initialised for every call of a().   */
    static seq_t dp_buf[4096];
    static seq_t next_buf[4096];
    seq_t *dp = dp_buf;
    seq_t *next = next_buf;

    for (int i = 0; i < 4096; ++i) {
        dp[i] = 1;          /* after the first two rows every pair is allowed */
        next[i] = 0;        /* clear the next buffer */
    }

    /* ------------------------------------------------------------
       Perform n transitions: each adds one more row to the array. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 4096; ++i) next[i] = 0;   /* clear */

        for (int state = 0; state < 4096; ++state) {
            seq_t cur = dp[state];
            if (!cur) continue;
            uint64_t mask = allowed_next[state];
            if (!mask) continue;
            uint64_t q = state & 63ULL;                 /* second row of the pair */
            while (mask) {
                int r = __builtin_ctzll(mask);          /* next row */
                next[(q << 6) | r] += cur;
                mask &= mask - 1;                       /* clear lowest set bit */
            }
        }

        /* swap dp and next for the next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    /* ------------------------------------------------------------
       Sum all counts to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < 4096; ++i) total += dp[i];
    return total;
}
