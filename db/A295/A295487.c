/*
 * A295487  Numbers that have exactly four representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A295487: numbers with exactly four unordered representations as a sum of
 * six nonnegative squares. The code enumerates integers, counts unordered
 * partitions into at most six positive squares by dynamic programming,
 * and selects those whose total representation count equals 4.
 * (Zero squares are added implicitly to reach six terms.)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool init_done = false;
static seq_t *seq_vals  = NULL;
static size_t seq_len   = 0;

/* Pre‑compute the sequence up to a safe upper bound (greater than 5408). */
static void init_sequence(void)
{
    /* Upper bound larger than the known maximum of the sequence. */
    const idx_t MAX_N = 6000;

    /* dp[p][s] – number of unordered partitions of s into exactly p positive squares. */
    static uint32_t dp[7][6001] = {0};
    dp[0][0] = 1;

    /* Enumerate all positive squares up to MAX_N. */
    for (idx_t i = 1; (idx_t)i * i <= MAX_N; ++i) {
        idx_t sq = (idx_t)i * i;
        for (idx_t s = sq; s <= MAX_N; ++s) {
            for (int p = 1; p <= 6; ++p) {
                dp[p][s] += dp[p - 1][s - sq];
            }
        }
    }

    /* Build the list of integers with exactly four representations. */
    /* Start with a reasonable capacity and grow as needed. */
    size_t cap = 128;
    seq_vals = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_len  = 0;

    for (idx_t n = 1; n <= MAX_N; ++n) {
        uint32_t total = 0;
        for (int p = 0; p <= 6; ++p) {
            total += dp[p][n];
        }
        if (total == 4) {
            if (seq_len == cap) {
                cap <<= 1;
                seq_vals = (seq_t *)realloc(seq_vals, cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)n;
        }
    }

    init_done = true;
}

/* Indexed from 1: a(1) is 17, a(2) is 18, etc. */
seq_t a(idx_t n)
{
    if (!init_done) {
        init_sequence();
    }
    if (n == 0 || n > seq_len) {
        return 0;          /* undefined index – return 0 */
    }
    return seq_vals[n - 1]; /* zero‑based array, one‑based index */
}
