/*
 * A283569  Number of nX5 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed via DP over rows. The count of n×5 0/1 arrays in which
 *            at most one 1 has more than one 1–neighbour (neighbours are
 *            left/right, up-left/up-right and down-left/down-right) equals
 *            a(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


static bool exc_done = false;
static uint8_t exc[32][32][32];

static void precompute_exc(void)
{
    if (exc_done) return;
    for (uint32_t a = 0; a < 32; ++a) {
        for (uint32_t b = 0; b < 32; ++b) {
            for (uint32_t c = 0; c < 32; ++c) {
                uint8_t e = 0;
                for (uint32_t j = 0; j < 5; ++j) if (b & (1u << j)) {
                    uint8_t neigh = 0;
                    if (j > 0 && (b & (1u << (j - 1)))) neigh++;
                    if (j < 4 && (b & (1u << (j + 1)))) neigh++;
                    if (j > 0 && (a & (1u << (j - 1)))) neigh++;
                    if (j < 4 && (a & (1u << (j + 1)))) neigh++;
                    if (j > 0 && (c & (1u << (j - 1)))) neigh++;
                    if (j < 4 && (c & (1u << (j + 1)))) neigh++;
                    if (neigh > 1) e++;
                }
                exc[a][b][c] = e;
            }
        }
    }
    exc_done = true;
}

static seq_t dp[32][32][2];
static seq_t ndp[32][32][2];
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t current_len = 0;   /* number of rows processed in dp */

/* Compute a(n) from the current dp state (which represents a rows already
   processed).  Only rows with exactly one exceptional cell are counted. */
static seq_t compute_final_result(void)
{
    seq_t res = 0;
    for (uint32_t a = 0; a < 32; ++a) {
        for (uint32_t b = 0; b < 32; ++b) {
            for (int used = 0; used < 2; ++used) {
                seq_t cnt = dp[a][b][used];
                if (!cnt) continue;
                uint8_t e_last = exc[a][b][0];   /* no down neighbours */
                if (used + e_last == 1) {
                    res += cnt;
                }
            }
        }
    }
    return res;
}

static void ensure_upto(idx_t n)
{
    if (n <= current_len) return;
    precompute_exc();

    if (!cache) {
        cache_size = n + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
    } else if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)cache_size * sizeof(seq_t));
    }
    cache[0] = 0;

    /* initialise for the first row if starting from scratch */
    if (current_len == 0) {
        memset(dp, 0, sizeof(dp));
        for (uint32_t p = 0; p < 32; ++p) dp[0][p][0] = (seq_t)1;
        current_len = 1;
        cache[1] = compute_final_result();
    }

    for (idx_t len = current_len + 1; len <= n; ++len) {
        memset(ndp, 0, sizeof(ndp));
        for (uint32_t a = 0; a < 32; ++a) {
            for (uint32_t b = 0; b < 32; ++b) {
                for (int used = 0; used < 2; ++used) {
                    seq_t cnt = dp[a][b][used];
                    if (!cnt) continue;
                    for (uint32_t c = 0; c < 32; ++c) {
                        uint8_t e = exc[a][b][c];
                        int newused = used + e;
                        if (newused > 1) continue;
                        ndp[b][c][newused] += cnt;
                    }
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
        cache[len] = compute_final_result();
    }
    current_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; values below are undefined */
    ensure_upto(n);
    return cache[n];
}
