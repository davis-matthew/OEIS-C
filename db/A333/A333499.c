/*
 * A333499  Number of numbers the sum of whose digits' factorials equals n.
 * https://oeis.org/A333499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via dynamic programming on digit factorial sums */

/* OEIS A333499: Number of numbers the sum of whose digits' factorials equals n. */

#include <stdlib.h>

static const unsigned int fact[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

static seq_t *dp = NULL;        /* dp[s] = number of digit sequences (allowing leading zeros) with sum s */
static size_t dp_size = 0;      /* current allocated size of dp */

/* Ensure dp is computed for all sums up to n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (dp_size <= (size_t)n) {
        size_t old_size = dp_size;
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        dp = tmp;

        /* initialize newly allocated entries to zero */
        for (size_t i = old_size; i < new_size; ++i) dp[i] = 0;

        if (old_size == 0) {
            dp[0] = 1;   /* base case: empty sequence */
        }

        /* compute values from the first missing index up to n */
        size_t start = old_size;
        if (start == 0) start = 1;   /* avoid recomputing dp[0] */
        for (size_t s = start; s <= (size_t)n; ++s) {
            seq_t val = 0;
            for (int d = 0; d < 10; ++d) {
                unsigned int fd = fact[d];
                if (s >= fd) val += dp[s - fd];
            }
            dp[s] = val;
        }
        dp_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value below offset is undefined */
    compute_up_to(n);

    seq_t count = 0;
    /* numbers whose first digit is non‑zero */
    for (int d = 1; d <= 9; ++d) {
        unsigned int fd = fact[d];
        if (n >= fd) count += dp[n - fd];
    }
    /* include number 0 when n == 1 (0! = 1) */
    if (n == 1) count += 1;
    return count;
}
