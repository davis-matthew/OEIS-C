/*
 * A242633  Number of n-length words w over a 10-ary alphabet {a_1,...,a_10}
 *          such that w contains never more than j consecutive letters a_j
 *          (for 1<=j<=10).
 * https://oeis.org/A242633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of length‑n words over a 10‑ary alphabet
 *            {a₁,…,a₁₀} such that w contains never more than j consecutive
 *            copies of a_j (1 ≤ j ≤ 10). The recurrence implemented below is
 *            the DP on the automaton described in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define ST 55   /* total number of states (j=1..10, run 1..j) */

/* memoisation structures */
static seq_t *dp   = NULL; /* counts for current length */
static seq_t *newdp = NULL;/* buffer for next length   */
static seq_t *ans  = NULL; /* memoised sequence values */
static size_t ansCap = 0;   /* allocated capacity of ans */
static idx_t  ansMax = 0;   /* maximum index computed so far */

/* state mapping tables */
static int letter_of[ST];
static int run_of[ST];
static int offset[11]; /* offset[l] = sum_{k<l} (k-1)  */

/* lazy initialisation of the automaton and memoisation buffers */
static void init(void)
{
    if (dp != NULL) return;             /* already initialised */

    /* allocate DP buffers */
    dp    = (seq_t *)calloc(ST, sizeof(seq_t));
    newdp = (seq_t *)calloc(ST, sizeof(seq_t));

    /* build state mapping and letter offsets */
    offset[1] = 0;
    for (int j = 2; j <= 10; ++j)
        offset[j] = offset[j-1] + (j-1);
    for (int l = 1; l <= 10; ++l)
        for (int r = 1; r <= l; ++r) {
            int idx = offset[l] + (r-1);
            letter_of[idx] = l;
            run_of[idx]    = r;
        }

    /* allocate answer array with initial capacity */
    ansCap = 1;                       /* holds index 0 */
    ans    = (seq_t *)calloc(ansCap, sizeof(seq_t));

    /* base values */
    ans[0] = (seq_t)1;                /* a(0) = 1 */
    ansMax = 0;
}

/* grow the answer array if necessary */
static void ensure_ans_capacity(idx_t n)
{
    if (n < ansCap) return;
    size_t newCap = ansCap ? ansCap : 1;
    while (newCap <= n) newCap <<= 1;
    ans = (seq_t *)realloc(ans, newCap * sizeof(seq_t));
    ansCap = newCap;
}

/* main function that returns a(n) */
seq_t a(idx_t n)
{
    init();                           /* lazy static initialisation */

    if (n <= ansMax)                  /* already computed */
        return ans[n];

    /* make sure we have space for the new answer */
    ensure_ans_capacity(n);

    /* build the sequence up to n */
    for (idx_t len = ansMax + 1; len <= n; ++len) {
        if (len == 1) {                /* initialisation for length 1 */
            for (int s = 0; s < ST; ++s) dp[s] = 0;
            for (int l = 1; l <= 10; ++l)
                dp[offset[l]] = (seq_t)1;     /* run length 1 */
            seq_t total = 0;
            for (int s = 0; s < ST; ++s) total += dp[s];
            ans[1] = total;
            ansMax = 1;
            continue;
        }

        /* general transition from length len-1 to len */
        for (int s = 0; s < ST; ++s) newdp[s] = 0;

        for (int s = 0; s < ST; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            int l = letter_of[s];
            int r = run_of[s];

            /* continue same letter if possible */
            if (r < l) {
                int ns = offset[l] + r;   /* new run length r+1 */
                newdp[ns] += val;
            }

            /* switch to a different letter */
            for (int m = 1; m <= 10; ++m) {
                if (m == l) continue;
                int ns = offset[m];        /* run length = 1 */
                newdp[ns] += val;
            }
        }

        /* sum up the counts for this length */
        seq_t total = 0;
        for (int s = 0; s < ST; ++s) total += newdp[s];
        ans[len] = total;

        /* prepare for next iteration: dp = newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
        ansMax = len;
    }

    return ans[n];
}
