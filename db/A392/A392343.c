/*
 * A392343  Numbers that are not the sum of at most five 4-full numbers.
 * https://oeis.org/A392343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_four_full
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A392343: Numbers that are not the sum of at most five 4‑full numbers. */
/*  Computed by brute‑force search for the required terms.                */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_four_full.h"

/* ---------- configuration ---------- */
#define MAX_SEQ_CAP 2000000      /* maximum expected number of terms          */
#define INITIAL_BOUND 200000     /* starting upper bound for the DP            */
#define MAX_COINS    1000        /* safety limit for stored 4‑full numbers      */

/* ---------- static storage ---------- */
static seq_t  seq[MAX_SEQ_CAP];      /* the sequence values                        */
static seq_t  temp_seq[MAX_SEQ_CAP]; /* temporary buffer for rebuilding the seq    */
static size_t seq_len = 0;           /* current number of stored terms             */

static idx_t  bound   = INITIAL_BOUND; /* current bound for the search             */

/* Rebuild the sequence up to the current bound */
static void rebuild_sequence(void)
{
    uint32_t coins[MAX_COINS];
    size_t coin_cnt = 0;

    /* generate all 4‑full numbers <= bound */
    for (uint32_t v = 1; v <= (uint32_t)bound; ++v) {
        if (is_four_full(v)) {
            if (coin_cnt < MAX_COINS) {
                coins[coin_cnt++] = v;
            }
        }
    }

    /* DP for sums with up to 5 coins */
    size_t cols = (size_t)bound + 1;
    size_t rows = 6;
    size_t dp_size = rows * cols;
    char *dp = (char *)calloc(dp_size, 1);          /* zeroed */
    if (!dp) abort();
    dp[0] = 1;                                      /* dp[0][0] = 1 */

    for (size_t idx = 0; idx < coin_cnt; ++idx) {
        uint32_t c = coins[idx];
        for (int k = 1; k <= 5; ++k) {
            char *prev = dp + (k-1)*cols;
            char *curr = dp + k*cols;
            for (uint32_t s = 0; s + c <= bound; ++s) {
                if (prev[s]) curr[s + c] = 1;
            }
        }
    }

    /* compute reachability */
    char *reach = (char *)calloc(cols, 1);
    if (!reach) abort();
    for (size_t k = 0; k <= 5; ++k) {
        char *row = dp + k*cols;
        for (size_t s = 0; s < cols; ++s) {
            if (row[s]) reach[s] = 1;
        }
    }

    /* rebuild seq from scratch */
    size_t new_len = 0;
    for (uint32_t v = 1; v <= (uint32_t)bound && new_len < MAX_SEQ_CAP; ++v) {
        if (!reach[v]) temp_seq[new_len++] = (seq_t)v;
    }

    /* copy to seq buffer and update length */
    for (size_t i = 0; i < new_len; ++i) seq[i] = temp_seq[i];
    seq_len = new_len;

    free(dp);
    free(reach);
}

/* Ensure the sequence has at least n terms computed */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;
    /* rebuild with the current bound until we have enough terms */
    while (seq_len < (size_t)n) {
        rebuild_sequence();
        if (seq_len >= (size_t)n) break;
        bound <<= 1;  /* double the bound and try again */
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined offset, return 0 */
    ensure_terms(n);
    return seq[(size_t)n - 1];
}
