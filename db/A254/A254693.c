/*
 * A254693  Number of length n+4 0..3 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 19, u64 n <= 43, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of length n+4 0..3 arrays with every five consecutive terms having the
   maximum of some two terms equal to the minimum of the remaining three terms. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Pre–computed validity of a 5‑tuple. 5 digits 0..3 in base‑4 encode a mask
   0..1023, the mask is (d0<<8)+(d1<<6)+(d2<<4)+(d3<<2)+d4 where d0 is the
   oldest. */
static bool valid_tbl[1024];
static bool valid_inited = false;
static inline void init_valid(void)
{
    if (valid_inited) return;
    for (int mask = 0; mask < 1024; ++mask) {
        int d[5];
        int t = mask;
        for (int i = 4; i >= 0; --i) {      /* d[0] = oldest, d[4] = newest */
            d[i] = t & 3;
            t >>= 2;
        }
        bool ok = false;
        for (int i = 0; i < 5 && !ok; ++i)
            for (int j = i + 1; j < 5 && !ok; ++j) {
                int mx = d[i] > d[j] ? d[i] : d[j];
                int mn = 3;
                for (int k = 0; k < 5; ++k)
                    if (k != i && k != j && d[k] < mn)
                        mn = d[k];
                if (mx == mn) ok = true;
            }
        valid_tbl[mask] = ok;
    }
    valid_inited = true;
}

/* DP state: the last up to four digits, encoded as a base‑4 number.
   256 possible states. */
static seq_t dp[256];
static seq_t nextdp[256];
static idx_t cur_len = 0;                 /* length represented by dp */

/* Memoised results for a(n). results[0] is unused. */
static seq_t *results = NULL;
static idx_t results_len = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(0) not used by OEIS; return 0 for safety. */
    if (n == 0) return 0;

    /* One‑time initialisation. */
    static bool initialized = false;
    if (!initialized) {
        init_valid();
        for (int i = 0; i < 256; ++i) dp[i] = 0;
        dp[0] = 1;   /* empty sequence */
        cur_len = 0;
        initialized = true;
    }

    /* Ensure the results array is large enough. */
    if (!results || results_len <= n) {
        idx_t new_cap = n + 1;
        seq_t *new_res = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (results) {
            for (idx_t i = 0; i < results_len; ++i) new_res[i] = results[i];
            free(results);
        }
        for (idx_t i = results_len; i < new_cap; ++i) new_res[i] = 0;
        results = new_res;
        results_len = new_cap;
    }

    /* Target length of the sequences: L = n + 4. */
    idx_t target_len = n + 4;

    /* Grow the DP until the target length is reached. */
    for (; cur_len < target_len; ++cur_len) {
        /* Initialise nextdp. */
        for (int i = 0; i < 256; ++i) nextdp[i] = 0;

        int l = (int)cur_len;          /* previous length */
        bool check = l >= 4;           /* whether to test the 5‑tuple */

        for (int s = 0; s < 256; ++s) {
            seq_t c = dp[s];
            if (c == 0) continue;
            for (int d = 0; d < 4; ++d) {
                int mask = (s << 2) + d;           /* encode 5‑tuple */
                if (check && !valid_tbl[mask]) continue;
                int new_state = ((s << 2) & 0xFF) + d;
                nextdp[new_state] += c;
            }
        }

        /* Swap dp and nextdp. */
        for (int i = 0; i < 256; ++i) dp[i] = nextdp[i];

        /* After the swap, dp represents sequences of length cur_len+1. */
        ++l; /* l now equals new length */

        /* If length >= 5 we can record a(n). */
        if (l >= 5) {
            idx_t m = l - 4;            /* m = n for this length */
            seq_t total = 0;
            for (int i = 0; i < 256; ++i) total += dp[i];
            results[m] = total;
        }
    }

    return results[n];
}
