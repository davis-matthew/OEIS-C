/*
 * A295485  Numbers that have exactly two representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A295485 – Numbers that have exactly two representations as
    a sum of six nonnegative squares.
    The function precomputes all such numbers up to a bound (here 100 000)
    and returns the n‑th term.
    Unordered representations are counted; zeros are used as needed to
    reach exactly six terms.  The implementation is efficient enough for
    thousands of calls.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state for the pre‑computed list */
static seq_t *list   = NULL;          /* array of all terms */
static idx_t  len    = 0;             /* number of terms */
static bool   ready  = false;         /* flag whether precomputation finished */

#define MAX_SUM ((idx_t)100000)

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!ready) {
        ready = true;

        /* ---------- 1. dynamic programming for unordered sums of squares
             dp[t][s] = number of ways to write s as a sum of exactly t
             positive square numbers, ignoring order --------------------------------- */
        const size_t dp_size = (size_t)(7 * (MAX_SUM + 1));
        seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
        if (!dp) return 0;   /* allocation failure – unlikely */

        /* Helper macro to access the two‑dimensional array */
#define DP(t,s) dp[ ((size_t)(t)*(MAX_SUM + 1)) + (size_t)(s) ]

        DP(0,0) = (seq_t)1;  /* one way to reach 0 with 0 squares */

        for (idx_t i = 1; (idx_t)i * i <= MAX_SUM; ++i) {
            idx_t sq = i * i;                /* current square value */
            for (idx_t s = sq; s <= MAX_SUM; ++s) {
                for (int t = 1; t <= 6; ++t) {
                    /* add ways that use one more square of value sq */
                    DP(t,s) += DP(t-1, s - sq);
                }
            }
        }

        /* ---------- 2. count representations with at most six squares
             r6[s] = Σ_{t=0..6} dp[t][s]  --------------------------------- */
        seq_t *r6 = (seq_t *)calloc((size_t)(MAX_SUM + 1), sizeof(seq_t));
        if (!r6) { free(dp); return 0; }

        for (idx_t s = 0; s <= MAX_SUM; ++s) {
            seq_t total = (seq_t)0;
            for (int t = 0; t <= 6; ++t) {
                total += DP(t,s);
            }
            r6[s] = total;
        }

        /* ---------- 3. collect all positive s with exactly two representations */
        idx_t cnt = 0;
        for (idx_t s = 1; s <= MAX_SUM; ++s) {
            if (r6[s] == (seq_t)2) ++cnt;
        }

        list = (seq_t *)malloc((size_t)cnt * sizeof(seq_t));
        if (!list) { free(dp); free(r6); return 0; }

        idx_t pos = 0;
        for (idx_t s = 1; s <= MAX_SUM; ++s) {
            if (r6[s] == (seq_t)2) {
                list[pos++] = (seq_t)s;
            }
        }

        free(dp);
        free(r6);
        #undef DP

        len = cnt;
    }

    /* ---------- 4. answer the query ------------------------------------------------ */
    if (n < 1 || (idx_t)(n-1) >= len) return 0;   /* out of range – for completeness */
    return list[n-1];
}
