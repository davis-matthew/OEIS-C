/*
 * A246895  Number of length 3+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 2*a(n-1) +4*a(n-2) -10*a(n-3)
 *            -5*a(n-4) +20*a(n-5) -20*a(n-7) +5*a(n-8) +10*a(n-9) -4*a(n-10)
 *            -2*a(n-11) +a(n-12) with initial values a(1) … a(12) computed by
 *            a DP over 7‑length arrays (values 0..n, every 5‑term window
 *            contains a pair summing to n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 196 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------------------
   The following routine computes a(1) … a(12) by an explicit DP over all
   7‑length arrays.  For each n the DP keeps a *state* describing the last
   four elements.  From a state we try all possible new elements; if the
   newly completed 5‑term window has a complementary pair we accept the
   transition.  At the end the total of all final states gives a(n).
   -------------------------------------------------------------------------- */
static void init_first12(seq_t *memo)
{
    const int MAX_N = 12;

    for (int n = 1; n <= MAX_N; ++n) {
        int base = n + 2;          /* base for state encoding: values 0..n plus sentinel */
        int sentinel = n + 1;      /* sentinel value for "not yet filled" */

        /* Number of possible 4‑digit states in base `base`. */
        size_t stateSize = 1;
        for (int i = 0; i < 4; ++i)
            stateSize *= (size_t)base;

        /* Allocate DP arrays and a table for decoding states. */
        seq_t *dp_prev = mem_new(seq_t, stateSize);
        seq_t *dp_curr = mem_new(seq_t, stateSize);
        /* decoded[s] = {a, b, c, d} for state index s. */
        int *decoded = (int*)malloc(stateSize * 4 * sizeof(int));
        for (size_t idx = 0; idx < stateSize; ++idx) {
            size_t tmp = idx;
            decoded[idx * 4 + 3] = tmp % base; tmp /= base;
            decoded[idx * 4 + 2] = tmp % base; tmp /= base;
            decoded[idx * 4 + 1] = tmp % base; tmp /= base;
            decoded[idx * 4 + 0] = tmp % base;
        }

        /* Initial state: all sentinel values. */
        size_t init_index = ((size_t)sentinel * base + sentinel) * base + sentinel;
        init_index = init_index * base + sentinel;
        dp_prev[init_index] = (seq_t)1;

        size_t basePow3 = (size_t)base * base * base;  /* Used for state transition. */

        /* Build DP for 7 positions. */
        for (int pos = 1; pos <= 7; ++pos) {
            mem_set(dp_curr, 0, stateSize * sizeof(seq_t));

            for (size_t s = 0; s < stateSize; ++s) {
                seq_t cnt = dp_prev[s];
                if (cnt == 0)
                    continue;

                /* Retrieve the four last values of this state. */
                int a = decoded[s * 4 + 0];
                int b = decoded[s * 4 + 1];
                int c = decoded[s * 4 + 2];
                int d = decoded[s * 4 + 3];

                if (pos >= 5) {   /* We must check the new 5‑term window. */
                    for (int val = 0; val <= n; ++val) {
                        /* Quick pair test for the window {a,b,c,d,val}. */
                        int w0 = a, w1 = b, w2 = c, w3 = d, w4 = val;
                        int ok =
                            (w0 + w1 == n) || (w0 + w2 == n) || (w0 + w3 == n) || (w0 + w4 == n) ||
                            (w1 + w2 == n) || (w1 + w3 == n) || (w1 + w4 == n) ||
                            (w2 + w3 == n) || (w2 + w4 == n) ||
                            (w3 + w4 == n);
                        if (!ok)
                            continue;

                        size_t newState = (s % basePow3) * base + val;
                        dp_curr[newState] += cnt;
                    }
                } else {          /* No window check yet. */
                    for (int val = 0; val <= n; ++val) {
                        size_t newState = (s % basePow3) * base + val;
                        dp_curr[newState] += cnt;
                    }
                }
            }
            /* Swap DP buffers for next position. */
            seq_t *tmp = dp_prev;
            dp_prev = dp_curr;
            dp_curr = tmp;
        }

        /* Sum all final states to obtain a(n). */
        seq_t total = 0;
        for (size_t s = 0; s < stateSize; ++s)
            total += dp_prev[s];
        memo[(idx_t)n] = total;

        mem_free(dp_prev);
        mem_free(dp_curr);
        free(decoded);
    }
}

/* Forward declaration of helper routine that computes the first 12 terms. */
static void init_first12(seq_t *memo);

/* Main function required by the judge. */
seq_t a(idx_t n)
{
    static seq_t *memo = NULL;      /* memoization array, index 0 unused. */
    static idx_t memo_cap = 0;      /* current capacity (size of memo). */

    /* Initialise the table with the first 12 terms at the very first call. */
    if (memo_cap == 0) {
        memo_cap = 13;                       /* indices 0 … 12  (0 unused) */
        memo = mem_new(seq_t, (size_t)memo_cap);
        init_first12(memo);
    }

    /* Simple lookup for the initial block. */
    if (n <= 12)
        return memo[(idx_t)n];

    /* Extend the memo table if a larger index is requested. */
    if (n >= memo_cap) {
        seq_t *new_mem = mem_resize(seq_t, memo, (size_t)(n + 1));
        if (!new_mem)     /* Allocation failure – return 0. */
            return (seq_t)0;
        memo = new_mem;
        for (idx_t k = memo_cap; k <= n; ++k) {
            seq_t a1  = memo[k - 1];
            seq_t a2  = memo[k - 2];
            seq_t a3  = memo[k - 3];
            seq_t a4  = memo[k - 4];
            seq_t a5  = memo[k - 5];
            seq_t a7  = memo[k - 7];
            seq_t a8  = memo[k - 8];
            seq_t a9  = memo[k - 9];
            seq_t a10 = memo[k -10];
            seq_t a11 = memo[k -11];
            seq_t a12 = memo[k -12];

            seq_t sumPos = a1 * 2 + a2 * 4 + a5 * 20 + a8 * 5 + a9 * 10 + a12 * 1;
            seq_t sumNeg = a3 * 10 + a4 * 5 + a7 * 20 + a10 * 4 + a11 * 2;
            memo[k] = sumPos - sumNeg;          /* unsigned wrap is safe; sumPos ≥ sumNeg. */
        }
        memo_cap = n + 1;
    }
    return memo[n];
}
