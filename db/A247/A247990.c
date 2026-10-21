/*
 * A247990  Number of length n+5 0..3 arrays with no disjoint triples in any
 *          consecutive six terms having the same sum.
 * https://oeis.org/A247990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n+5 over {0,1,2,3} * with no
 *            disjoint triples in any consecutive six terms having the same
 *            sum
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 41, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define STATES 1024   /* 4^5 */
#define TUPLES 4096   /* 4^6 */

/* Forward declarations */
static void init(void);
static void ensure_up_to(idx_t n);

/* Global tables (static) */
static bool bad[TUPLES];
static bool trans_ok[STATES][4];
static unsigned short trans_new[STATES][4];
static seq_t *dp_cur = NULL;     /* DP for the current length */
static seq_t *dp_next = NULL;    /* DP for the next length */
static seq_t *answers = NULL;    /* answers[1..max_n] */
static size_t ans_cap = 0;
static idx_t cached_n = 0;
static bool initialized = false;

/*--------------------------------------------------------------------
 * Pre‑compute tables once.
 *--------------------------------------------------------------------*/
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* -- pre‑compute bad 6‑tuples ---------------------------------
     *   A 6‑tuple is bad if there exists two disjoint triples with equal sum.
     */
    for (int t = 0; t < TUPLES; ++t) {
        unsigned char digit[6];
        int temp = t;
        for (int i = 0; i < 6; ++i) {
            digit[i] = (unsigned char)(temp & 3);
            temp >>= 2;
        }
        bool bad_flag = false;
        for (int mask = 0; mask < 64; ++mask) {
            if (__builtin_popcount(mask) != 3) continue;
            int sum1 = 0, sum2 = 0;
            for (int i = 0; i < 6; ++i) {
                if (mask & (1 << i)) sum1 += digit[i];
                else                  sum2 += digit[i];
            }
            if (sum1 == sum2) { bad_flag = true; break; }
        }
        bad[t] = bad_flag;
    }

    /* -- pre‑compute transitions ------------------------------------
     *   For each state (last 5 digits) and next digit d, compute if
     *   the resulting 6‑tuple is allowed and the new state.
     */
    for (int s = 0; s < STATES; ++s) {
        for (int d = 0; d < 4; ++d) {
            int t = s | (d << 10);   /* 6‑tuple index */
            if (!bad[t]) {
                trans_ok[s][d] = true;
                trans_new[s][d] = (unsigned short)((s >> 2) | (d << 8));
            } else {
                trans_ok[s][d] = false;
            }
        }
    }

    /* -- initialize DP for length 6 --------------------------------
     *   Longest needed DP step will grow from length 6 upwards.
     */
    dp_cur  = (seq_t *)malloc(STATES * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));
    if (!dp_cur || !dp_next) { abort(); }

    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;

    for (int t = 0; t < TUPLES; ++t) {
        if (!bad[t]) {
            int state = t & 0x3FF;  /* last 5 digits */
            dp_cur[state] += 1;
        }
    }

    /* -- pre‑allocate answers array -------------------------------
     *   We'll enlarge it lazily.
     */
    ans_cap = 1024;
    answers = (seq_t *)malloc(ans_cap * sizeof(seq_t));
    if (!answers) abort();

    /* compute answer for n=1 (length 6) */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) total += dp_cur[i];
    answers[1] = total;
    cached_n = 1;
}

/*--------------------------------------------------------------------
 * Extend DP until n is covered.
 *--------------------------------------------------------------------*/
static void ensure_up_to(idx_t n)
{
    if (n <= cached_n) return;
    init();

    for (idx_t idx = cached_n + 1; idx <= n; ++idx) {
        /* grow answers array if needed */
        if (idx >= ans_cap) {
            size_t new_cap = ans_cap * 2;
            seq_t *tmp = (seq_t *)realloc(answers, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            answers = tmp;
            ans_cap = new_cap;
        }

        /* compute next DP layer */
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            if (dp_cur[s] == 0) continue;
            seq_t cnt = dp_cur[s];
            for (int d = 0; d < 4; ++d) {
                if (trans_ok[s][d]) {
                    unsigned short ns = trans_new[s][d];
                    dp_next[ns] += cnt;
                }
            }
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;

        /* compute total for current length */
        seq_t total = 0;
        for (int i = 0; i < STATES; ++i) total += dp_cur[i];
        answers[idx] = total;
    }
    cached_n = n;
}

/*--------------------------------------------------------------------
 * Public function.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset, not used */
    ensure_up_to(n);
    return answers[n];
}
