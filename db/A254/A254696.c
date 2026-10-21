/*
 * A254696  Number of length n+4 0..6 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n+4 0..6 arrays with every five consecutive
 *            terms having the maximum of some two terms equal to the minimum
 *            of the remaining three terms
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 31, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses DP on last four elements
   with state size 7^4 = 2401.  The transition
   table and validity of a 5‑tuple are precomputed. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 2401   /* 7^4 */

static unsigned char    valid_table[STATE_COUNT][7];
static unsigned short   next_table [STATE_COUNT][7];
static bool             precomputed = false;

static void init_precompute(void)
{
    if (precomputed) return;
    precomputed = true;

    for (int s = 0; s < STATE_COUNT; ++s) {
        int tmp = s;
        int v3 = tmp % 7; tmp /= 7;
        int v2 = tmp % 7; tmp /= 7;
        int v1 = tmp % 7; tmp /= 7;
        int v0 = tmp % 7;

        for (int v = 0; v < 7; ++v) {
            int w[5] = { v0, v1, v2, v3, v };
            bool ok = false;

            /* Check 10 possible pairs (p,q). */
            for (int p = 0; !ok && p < 5; ++p) {
                for (int q = p + 1; !ok && q < 5; ++q) {
                    int maxpq = (w[p] > w[q]) ? w[p] : w[q];
                    int minrest = 7;   /* larger than any value 0..6 */
                    for (int r = 0; r < 5; ++r) {
                        if (r == p || r == q) continue;
                        if (w[r] < minrest) minrest = w[r];
                    }
                    if (maxpq == minrest) ok = true;
                }
            }
            valid_table[s][v] = ok ? 1 : 0;

            /* New state after appending v: drop v0, shift left */
            unsigned short ns =
                (unsigned short)(((v1 * 7 + v2) * 7 + v3) * 7 + v);
            next_table[s][v] = ns;
        }
    }
}

seq_t a(idx_t n)
{
    static idx_t          max_n  = 0;
    static seq_t*         dp_cur = NULL;
    static seq_t*         dp_next = NULL;
    static seq_t*         values = NULL;
    static size_t         val_cap = 0;
    static idx_t          cur_len = 0;   /* length represented by dp_cur */

    init_precompute();

    /* Allocate/initialize structures on first call */
    if (!dp_cur) {
        dp_cur  = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);
        dp_next = (seq_t*)malloc(sizeof(seq_t) * STATE_COUNT);
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)1; /* all 4‑tuples */

        /* values[0] unused, values[1] will hold first term */
        val_cap = 1;
        values  = (seq_t*)malloc(sizeof(seq_t) * val_cap);
        values[0] = 0;
        cur_len = 4;
    }

    /* Ensure values array is large enough */
    if ((size_t)(n + 1) > val_cap) {
        size_t new_cap = val_cap ? val_cap : 1;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        values = (seq_t*)realloc(values, sizeof(seq_t) * new_cap);
        val_cap = new_cap;
    }

    /* Extend DP until we have a(n) */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        memset(dp_next, 0, sizeof(seq_t) * STATE_COUNT);

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp_cur[s];
            if (!cnt) continue;
            for (int v = 0; v < 7; ++v) {
                if (!valid_table[s][v]) continue;
                unsigned short ns = next_table[s][v];
                dp_next[ns] += cnt;
            }
        }

        /* Prepare for next length */
        cur_len++;
        seq_t* tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;

        /* Sum all counts to get a(k) */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i) total += dp_cur[i];
        values[k] = total;
    }

    max_n = n;
    return values[n];
}
