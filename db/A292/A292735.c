/*
 * A292735  Numbers in which 5 outnumbers all other digits together.
 * https://oeis.org/A292735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose decimal representation
   contains more 5's than all other digits combined. */

/* The algorithm enumerates by digit length, counting how many
   sequences satisfy the condition using combinatorial formulas.
   Then it constructs the nth number digit‑by‑digit, using the
   same combinatorial counts to skip entire ranges of candidates. */

#include <stddef.h>
#include <stdint.h>

/* Pre‑compute binomial coefficients and powers of 9 and 10
   up to a sufficiently large length (here 64).  The table
   is constructed once on the first call. */
#define MAX_LEN 64

static idx_t  choose_tbl[MAX_LEN + 1][MAX_LEN + 1];
static seq_t  pow9_tbl[MAX_LEN + 1];
static seq_t  pow10_tbl[MAX_LEN + 1];
static int    tables_ready = 0;

static void init_tables(void)
{
    if (tables_ready) return;
    /* binomial coefficients */
    for (idx_t n = 0; n <= MAX_LEN; ++n) {
        choose_tbl[n][0] = choose_tbl[n][n] = 1;
        for (idx_t k = 1; k < n; ++k) {
            choose_tbl[n][k] = choose_tbl[n-1][k-1] + choose_tbl[n-1][k];
        }
    }
    /* powers of 9 and 10 */
    pow9_tbl[0] = 1;
    pow10_tbl[0] = 1;
    for (idx_t i = 1; i <= MAX_LEN; ++i) {
        pow9_tbl[i] = pow9_tbl[i-1] * 9;
        pow10_tbl[i] = pow10_tbl[i-1] * 10;
    }
    tables_ready = 1;
}

/* Count completions for the remaining `rem` positions,
   given the current number of 5's `c5` and total length `len`. */
static seq_t count_completions(idx_t rem, idx_t c5, idx_t len)
{
    idx_t min_fives = (len >> 1) + 1;          /* floor(len/2)+1 */
    int64_t need = (int64_t)min_fives - (int64_t)c5;
    if (need <= 0)        /* already enough 5's, any remaining digits work */
        return pow10_tbl[rem];
    if ((idx_t)need > rem)/* impossible to reach the minimum */
        return 0;
    seq_t total = 0;
    for (idx_t t = (idx_t)need; t <= rem; ++t) {
        total += choose_tbl[rem][t] * pow9_tbl[rem - t];
    }
    return total;
}

/* Main sequence function */
seq_t a(idx_t n) /* n >= 1 */
{
    init_tables();

    /* --- find the digit length of the nth term --- */
    idx_t len = 0;
    seq_t cumulative = 0;
    for (;;) {
        ++len;
        seq_t total_len = 0;
        idx_t min_fives = (len >> 1) + 1;
        for (idx_t k = min_fives; k <= len; ++k) {
            seq_t first5    = choose_tbl[len-1][k-1] * pow9_tbl[len - k];
            seq_t firstNot5 = (seq_t)8 * choose_tbl[len-1][k] * pow9_tbl[(len-1) - k];
            total_len += first5 + firstNot5;
        }
        if (cumulative + total_len >= n)
            break;
        cumulative += total_len;
    }

    /* --- rank within numbers of length `len` --- */
    seq_t rank = n - cumulative;          /* 1‑based */
    seq_t result = 0;
    idx_t c5 = 0;

    /* --- build the number digit by digit --- */
    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t rem = len - pos;
        for (int digit = 0; digit <= 9; ++digit) {
            if (pos == 1 && digit == 0)  /* leading zero not allowed */
                continue;
            idx_t new_c5 = c5 + (digit == 5 ? 1 : 0);
            seq_t combos = count_completions(rem, new_c5, len);
            if (rank > combos) {
                rank -= combos;
                continue;   /* try next digit */
            }
            /* this digit is chosen */
            result = result * 10 + (seq_t)digit;
            c5 = new_c5;
            break;
        }
    }
    return result;
}
