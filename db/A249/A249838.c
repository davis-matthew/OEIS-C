/*
 * A249838  Number of length n+4 0..2 arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 29, u64 n <= 66, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249838
   Number of length n+4 arrays over {0,1,2} with no five consecutive terms
   having the maximum of any two terms equal to the minimum of the remaining
   three terms.
   Computed by dynamic programming over 4‑symbol states (3^4 = 81 states). */
#include <stdbool.h>

static unsigned char allowed[81][3];
static bool allowed_inited = false;

/* Build the table of admissible 5‑tuples. */
static void init_allowed(void)
{
    for (int state = 0; state < 81; ++state) {
        int a[4];
        int t = state;
        a[3] = t % 3; t /= 3;
        a[2] = t % 3; t /= 3;
        a[1] = t % 3; t /= 3;
        a[0] = t % 3;

        for (int x = 0; x < 3; ++x) {
            int v[5] = { a[0], a[1], a[2], a[3], x };
            bool ok = true;
            for (int i = 0; i < 5 && ok; ++i) {
                for (int j = i + 1; j < 5; ++j) {
                    int maxv = v[i] > v[j] ? v[i] : v[j];
                    int minrem = 10;
                    for (int k = 0; k < 5; ++k) {
                        if (k == i || k == j) continue;
                        if (v[k] < minrem) minrem = v[k];
                    }
                    if (maxv == minrem) { ok = false; break; }
                }
            }
            allowed[state][x] = ok ? 1 : 0;
        }
    }
    allowed_inited = true;
}

/* Main routine. */
seq_t a(idx_t n)
{
    if (!allowed_inited) init_allowed();

    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    const idx_t N = n + 4;                /* length of the array */

    seq_t dp[81];
    seq_t nxt[81];

    /* All 4‑symbol prefixes are initially allowed (no constraints yet). */
    for (int s = 0; s < 81; ++s) dp[s] = (seq_t)1;

    /* Extend the array one symbol at a time, respecting the 5‑tuple rule. */
    for (idx_t pos = 5; pos <= N; ++pos) {
        for (int s = 0; s < 81; ++s) nxt[s] = (seq_t)0;

        for (int s = 0; s < 81; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;
            for (int x = 0; x < 3; ++x) {
                if (!allowed[s][x]) continue;
                int ns = (int)(((s % 27) * 3) + x);   /* shift left, add x */
                nxt[ns] += cur;
            }
        }
        for (int s = 0; s < 81; ++s) dp[s] = nxt[s];
    }

    /* Sum over all ending states to obtain the total number of valid arrays. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 81; ++s) result += dp[s];
    return result;
}
