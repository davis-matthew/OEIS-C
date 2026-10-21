/*
 * A249878  Number of length n+6 0..3 arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 15, u64 n <= 38, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249878 – number of length n+6 0..3 arrays with no forbidden 7‑window.
   The forbidden condition: in any 7 consecutive terms, the maximum of
   any three terms equals the minimum of the remaining four.
   The algorithm uses a dynamic program over the last six digits.
   States: 4^6 = 4096 6‑tuples (0..3). Transition from state `s`
   to state `s2` appends a new digit `x` (0..3) iff the resulting
   7‑tuple `(s,x)` is not forbidden.  The DP is evaluated lazily
   and cached so that subsequent calls are O(1).  */

/*  *  A  *  */
#include <stdbool.h>

/* ----------   constants   ---------- */
#define STATE_COUNT 4096   /* 4^6 states */
#define WINDOW_COUNT 16384 /* 4^7 windows */

/* ----------   static data   ---------- */
static bool forb[WINDOW_COUNT];           /* forbidden 7‑tuples   */
static bool forb_built = false;          /* were they built?     */

static seq_t a_counts[10001];            /* counts for n = 0..10000, n=0 unused   */
static idx_t max_computed = 0;           /* largest n that has been computed     */

static seq_t dp_cur[STATE_COUNT];        /* DP for current length (starts at 6)   */
static seq_t dp_next[STATE_COUNT];       /* DP for next length                  */
static seq_t *cur = dp_cur;              /* pointer to current DP array          */
static seq_t *next = dp_next;            /* pointer to next DP array             */

/* ----------  helpers   ---------- */
static void build_forbidden(void)
{
    for (int w = 0; w < WINDOW_COUNT; ++w) {
        int d[7];
        int t = w;
        for (int i = 0; i < 7; ++i) {        /* lift base‑4 digits */
            d[i] = t & 3;
            t >>= 2;
        }
        bool bad = false;
        for (int i = 0; i < 7 && !bad; ++i)
            for (int j = i + 1; j < 7 && !bad; ++j)
                for (int k = j + 1; k < 7 && !bad; ++k) {
                    int maxv = d[i];
                    if (d[j] > maxv) maxv = d[j];
                    if (d[k] > maxv) maxv = d[k];
                    int minv = 4;
                    for (int p = 0; p < 7; ++p)
                        if (p != i && p != j && p != k && d[p] < minv)
                            minv = d[p];
                    if (maxv == minv) bad = true;
                }
        forb[w] = bad;
    }
    forb_built = true;
}

/* ensure the sequence has been computed up to `n` (n≥1) */
static void ensure_memo(idx_t n)
{
    if (max_computed >= n) return;            /* already done */

    if (!forb_built) {                         /* first time */
        build_forbidden();

        /* all 6‑tuples admissible for length 6 */
        for (int s = 0; s < STATE_COUNT; ++s)
            dp_cur[s] = 1;

        cur = dp_cur;
        next = dp_next;
        max_computed = 0;
    }

    /* current length = max_computed + 6   */
    for (idx_t len = max_computed + 7; len <= n + 6; ++len) {
        /* zero next table */
        for (int s = 0; s < STATE_COUNT; ++s)
            next[s] = 0;

        /* propagate one digit */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            for (int x = 0; x < 4; ++x) {
                int w = (s << 2) | x;     /* 7‑digit window */
                if (forb[w]) continue;    /* forbidden */
                int s2 = ((s & 0x3FF) << 2) | x; /* new 6‑digit suffix */
                next[s2] += val;
            }
        }

        /* total number of admissible strings of this length */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            total += next[s];

        a_counts[len - 6] = total;     /* n = len - 6 */

        /* prepare for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }
    max_computed = n;
}

/* ----------   public routine   ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 */
    if (n > 10000) return 0;         /* out of pre‑computed range */
    ensure_memo(n);
    return a_counts[n];
}
