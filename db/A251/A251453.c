/*
 * A251453  Number of (n+1)X(2+1) 0..3 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over 3‑digit rows with
 *            an adjacency matrix. No closed form except the DP recurrence
 *            a_{k+1}(j) = Σ_i a_k(i) * adj[i][j]. Start with one row and
 *            perform n steps to obtain (n+1) rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Number of possible rows (3 base‑4 digits). */
enum { SZ = 64 };

/* Prime indicator for sums 0–12. */
static const bool is_prime_sum[13] = {
    false, /* 0 */
    false, /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    false, /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false, /* 9 */
    false, /*10 */
    true,  /*11 */
    false  /*12 */
};

/* Convert a row index 0..63 into its 3 base‑4 digits. */
static inline void row_digits(uint8_t d[3], size_t idx)
{
    d[0] = (uint8_t)(idx >> 4);           /* bits 4–5 */
    d[1] = (uint8_t)((idx >> 2) & 3);     /* bits 2–3 */
    d[2] = (uint8_t)(idx & 3);            /* bits 0–1 */
}

/* Adjacency table for compatible row pairs. */
static bool adj[SZ][SZ];
static bool adj_init = false;

/* Initialise the adjacency matrix once. */
static void init_adj(void)
{
    if (adj_init)
        return;

    uint8_t row[3], col[3];
    for (size_t a = 0; a < SZ; ++a) {
        row_digits(row, a);
        for (size_t b = 0; b < SZ; ++b) {
            row_digits(col, b);
            bool ok = true;
            for (size_t j = 0; j < 2; ++j) {
                unsigned sum = row[j] + row[j + 1] + col[j] + col[j + 1];
                if (!is_prime_sum[sum]) {
                    ok = false;
                    break;
                }
            }
            adj[a][b] = ok;
        }
    }
    adj_init = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_adj();

    seq_t cur[SZ];
    seq_t nxt[SZ];

    /* Rows of length 1: all 64 rows are allowed. */
    for (size_t i = 0; i < SZ; ++i)
        cur[i] = 1;

    /* Perform n transitions to obtain n+1 rows. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero nxt. */
        for (size_t j = 0; j < SZ; ++j)
            nxt[j] = 0;

        /* Update counts using the adjacency matrix. */
        for (size_t i = 0; i < SZ; ++i) {
            if (cur[i] == 0)
                continue;
            for (size_t j = 0; j < SZ; ++j) {
                if (adj[i][j])
                    nxt[j] += cur[i];
            }
        }

        /* Move to next step. */
        for (size_t i = 0; i < SZ; ++i)
            cur[i] = nxt[i];
    }

    /* Sum over all possible final rows. */
    seq_t result = 0;
    for (size_t i = 0; i < SZ; ++i)
        result += cur[i];

    return result;
}
