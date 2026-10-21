/*
 * A252633  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 0 3 4 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 0 3 4 6 or 7.
 * https://oeis.org/A252633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 55, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n)= number of (n+2)×3 arrays over {0,1,2,3} such that
   every 3×3 subblock has row sums and main diagonal sum in
   {0,3,4,6,7} and column sums and anti‑diagonal sum outside that set.
   Initial values: a(1)=572, a(2)=687, a(3)=1189, a(4)=2230, …  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Allowed sum set {0,3,4,6,7}.  Index by sum 0..9. */
static const bool S_allowed[10] = {
    true,  /* 0 */
    false, /* 1 */
    false, /* 2 */
    true,  /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true,  /* 6 */
    true,  /* 7 */
    false, /* 8 */
    false  /* 9 */
};

seq_t a(idx_t n)
{
    /* Special case: the sequence is defined for n>=1.  Undefined below. */
    if (n == 0) return (seq_t)0;

    const size_t N = (size_t)n + 2;     /* total number of rows */

    /* ---------- static data structures ---------- */
    static bool initialized = false;
    static uint64_t allowed_mask[64][64];         /* mask of admissible third rows */
    static short digits[64][3];                   /* digits[r][0..2] */
    static short row_sum[64];                     /* sum of digits of a row */

    /* DP arrays: counts for pairs of last two rows.  Size 64×64=4096. */
    static seq_t dp[4096];
    static seq_t next_dp[4096];
    static size_t dp_rows = 2;          /* number of rows currently processed */

    /* Results cache: results[N] = answer for N rows.  Index 0.. */
    static seq_t *results = NULL;
    static size_t results_cap = 0;
    static size_t results_len = 0;       /* highest N that has been computed */

    /* Allocate results buffer on first call. */
    if (results == NULL) {
        results_cap = 8;
        results = (seq_t*)calloc(results_cap, sizeof(seq_t));
        if (!results) return (seq_t)0;
    }

    /* Return cached answer if already known. */
    if (N <= results_len) return results[N];

    /* ---------- Initialise static tables if necessary ---------- */
    if (!initialized) {
        /* Pre–compute digits and row sums. */
        for (int r = 0; r < 64; ++r) {
            int x = r;
            digits[r][0] = x / 16;        /* most significant digit */
            x %= 16;
            digits[r][1] = x / 4;
            digits[r][2] = x % 4;
            row_sum[r] = digits[r][0] + digits[r][1] + digits[r][2];
        }

        /* Pre‑compute allowed transitions for every pair (r1,r2). */
        for (int r1 = 0; r1 < 64; ++r1) {
            for (int r2 = 0; r2 < 64; ++r2) {
                uint64_t mask = 0;
                /* Fast reject if any required sum impossible. */
                if (!S_allowed[row_sum[r1]] || !S_allowed[row_sum[r2]])
                    { allowed_mask[r1][r2] = 0; continue; }

                for (int r3 = 0; r3 < 64; ++r3) {
                    if (!S_allowed[row_sum[r3]]) continue;

                    /* Main diagonal */
                    int diag = digits[r1][0] + digits[r2][1] + digits[r3][2];
                    if (!S_allowed[diag]) continue;

                    /* Column sums must NOT be in S_allowed */
                    int col0 = digits[r1][0] + digits[r2][0] + digits[r3][0];
                    if (S_allowed[col0]) continue;
                    int col1 = digits[r1][1] + digits[r2][1] + digits[r3][1];
                    if (S_allowed[col1]) continue;
                    int col2 = digits[r1][2] + digits[r2][2] + digits[r3][2];
                    if (S_allowed[col2]) continue;

                    /* Anti‑diagonal must NOT be in S_allowed */
                    int antidiag = digits[r1][2] + digits[r2][1] + digits[r3][0];
                    if (S_allowed[antidiag]) continue;

                    mask |= (1ULL << r3);
                }
                allowed_mask[r1][r2] = mask;
            }
        }

        /* Initialise DP for two rows: one way to start with any pair of rows. */
        for (size_t i = 0; i < 4096; ++i) dp[i] = (seq_t)1;
        dp_rows = 2;
        initialized = true;
    }

    /* Ensure results array is large enough. */
    if (N + 1 > results_cap) {
        size_t new_cap = results_cap;
        while (N + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        memset(tmp + results_cap, 0, (new_cap - results_cap) * sizeof(seq_t));
        results = tmp;
        results_cap = new_cap;
    }

    /* Extend the DP until we reach N rows. */
    for (size_t cur = dp_rows + 1; cur <= N; ++cur) {
        /* Zero next_dp */
        for (size_t i = 0; i < 4096; ++i) next_dp[i] = (seq_t)0;

        /* Transition from each state (r1,r2). */
        for (size_t idx = 0; idx < 4096; ++idx) {
            seq_t val = dp[idx];
            if (val == (seq_t)0) continue;
            int r1 = (int)(idx / 64);
            int r2 = (int)(idx % 64);
            uint64_t mask = allowed_mask[r1][r2];
            while (mask) {
                uint64_t lowbit = mask & -mask;
                int r3 = (int)__builtin_ctzll(mask);
                size_t next_idx = ((size_t)r2 << 6) | (size_t)r3;
                next_dp[next_idx] += val;
                mask ^= lowbit;
            }
        }

        /* Swap dp arrays */
        for (size_t i = 0; i < 4096; ++i) dp[i] = next_dp[i];

        /* Compute total for current number of rows. */
        seq_t total = (seq_t)0;
        for (size_t i = 0; i < 4096; ++i) total += dp[i];
        results[cur] = total;
    }

    dp_rows = N;
    results_len = N;
    return results[N];
}
