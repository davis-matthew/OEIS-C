/*
 * A295038  Number of nX6 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1 or 2 1s.
 * https://oeis.org/A295038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays where every 1 has 1 or 2
 *            orthogonal 1‑neighbours. Implementation uses dynamic programming
 *            over two consecutive rows
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Horizontal neighbour count inside a row (left + right). */
static int h[64][6];

/* For each pair of previous rows (a, b) keep the list of next rows
   that keep the middle row b valid. */
static uint8_t  next_len[64][64];   /* how many such next rows */
static uint32_t next_ptr[64][64];   /* start index into next_arr */
static uint8_t  next_arr[262144];   /* concatenated list of next rows */

/* For the last row (b) with up row a and down row = 0. */
static uint8_t valid_mask[64][64];

/* One‑time precomputation of all auxiliary tables. */
static void init_precomputation(void)
{
    /* horizontal neighbours */
    for (int mask = 0; mask < 64; ++mask) {
        for (int j = 0; j < 6; ++j) {
            if ((mask >> j) & 1) {
                int left  = (j > 0) ? ((mask >> (j - 1)) & 1) : 0;
                int right = (j < 5) ? ((mask >> (j + 1)) & 1) : 0;
                h[mask][j] = left + right;
            } else {
                h[mask][j] = 0;
            }
        }
    }

    /* transition lists for (a,b) → c */
    uint32_t offset = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            uint8_t cnt = 0;
            for (int c = 0; c < 64; ++c) {
                bool ok = true;
                for (int j = 0; j < 6 && ok; ++j) {
                    if ((b >> j) & 1) {
                        int total = h[b][j]
                                    + ((a >> j) & 1)
                                    + ((c >> j) & 1);
                        if (!(total == 1 || total == 2)) ok = false;
                    }
                }
                if (ok) {
                    next_arr[offset + cnt] = (uint8_t)c;
                    ++cnt;
                }
            }
            next_len[a][b] = cnt;
            next_ptr[a][b] = offset;
            offset += cnt;
        }
    }

    /* validity of the last row (down = 0) */
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            bool ok = true;
            for (int j = 0; j < 6 && ok; ++j) {
                if ((b >> j) & 1) {
                    int total = h[b][j] + ((a >> j) & 1);
                    if (!(total == 1 || total == 2)) ok = false;
                }
            }
            valid_mask[a][b] = ok ? 1 : 0;
        }
    }
}

static bool precomp_done = false;

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!precomp_done) {
        init_precomputation();
        precomp_done = true;
    }

    static seq_t dp_cur[64][64];
    static seq_t dp_next[64][64];

    /* initialise dp_cur for the first row (prevprev = 0) */
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b)
            dp_cur[a][b] = 0;
    for (int b = 0; b < 64; ++b)
        dp_cur[0][b] = 1;

    /* add the remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* clear dp_next */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp_next[a][b] = 0;

        /* transition */
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t cnt = dp_cur[a][b];
                if (cnt == 0) continue;
                uint32_t ptr = next_ptr[a][b];
                uint8_t  len = next_len[a][b];
                for (uint8_t k = 0; k < len; ++k) {
                    int c = next_arr[ptr + k];
                    dp_next[b][c] += cnt;
                }
            }
        }

        /* swap dp_cur and dp_next */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp_cur[a][b] = dp_next[a][b];
    }

    /* sum over final states where the last row is valid (down = 0) */
    seq_t result = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            if (valid_mask[a][b])
                result += dp_cur[a][b];
        }
    }

    return result;
}
