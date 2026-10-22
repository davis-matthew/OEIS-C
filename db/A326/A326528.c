/*
 * A326528  Sum of the fifth largest parts of the partitions of n into 9
 *          squarefree parts.
 * https://oeis.org/A326528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over all partitions of n into 9 square‑free parts of
 *            the 5th largest part of the partition. Non‑decreasing order of
 *            parts is used to avoid double counting. The implementation uses
 *            a dynamic‑programming approach that incrementally selects
 *            square‑free parts and keeps track of the number...
 * Range:     u8 n <= 28, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation works for the built‑in integer types
   (uint8_t … uint64_t) and for the arbitrary‑precision class that
   provides +, -, *, /, %=, <, >, <=, >=, ==, !=, <<, >>, &, |, ^
   operators, as well as operator+= and operator-= for seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* ------- basic size handling ------------------------------------- */
    size_t N = (size_t)n;                 /* maximum sum needed */
    size_t SZ = N + 1;                    /* array length needed */

    /* ------- allocate 2‑D arrays for counts and sums ----------------- */
    seq_t **cnt = (seq_t**)malloc(10 * sizeof(seq_t*));
    seq_t **sum = (seq_t**)malloc(10 * sizeof(seq_t*));
    if (!cnt || !sum) return 0;          /* allocation failure */

    for (int k = 0; k < 10; ++k) {
        cnt[k] = (seq_t*)calloc(SZ, sizeof(seq_t));
        sum[k] = (seq_t*)calloc(SZ, sizeof(seq_t));
        if (!cnt[k] || !sum[k]) return 0; /* allocation failure */
    }

    cnt[0][0] = 1;                       /* one way to reach sum 0 with 0 parts */

    /* ------- build flag array for square‑free numbers ---------------- */
    bool *sqf = (bool*)calloc(SZ, sizeof(bool));
    if (!sqf) return 0;
    for (size_t i = 0; i <= N; ++i) sqf[i] = true;
    if (N >= 0) sqf[0] = false;          /* 0 is not a part */
    if (N >= 1) sqf[1] = true;
    size_t lim = (size_t)sqrt((double)N);
    for (size_t d = 2; d <= lim; ++d) {
        size_t sq = d * d;
        for (size_t j = sq; j <= N; j += sq)
            sqf[j] = false;
    }

    /* ------- DP over all square‑free values (unlimited multiplicity) */
    for (size_t v = 1; v <= N; ++v) {
        if (!sqf[v]) continue;
        seq_t sv = (seq_t)v;              /* cast to seq_t for later use */

        for (int k = 1; k <= 9; ++k) {
            seq_t *cntPrev = cnt[k-1];
            seq_t *cntCur  = cnt[k];
            seq_t *sumPrev = sum[k-1];
            seq_t *sumCur  = sum[k];

            for (size_t s = v; s <= N; ++s) {
                seq_t ways = cntPrev[s - v];
                if (!ways) continue;    /* skip zero contributions */

                cntCur[s] += ways;      /* update number of ways */

                if (k == 5) {
                    /* the v we are adding becomes the 5th part */
                    sumCur[s] += sumPrev[s - v] + sv * ways;
                } else {
                    /* 5th part already determined earlier (if k>=5) */
                    sumCur[s] += sumPrev[s - v];
                }
            }
        }
    }

    seq_t result = sum[9][N];

    /* ------- clean up ----------------------------------------------- */
    for (int k = 0; k < 10; ++k) {
        free(cnt[k]);
        free(sum[k]);
    }
    free(cnt);
    free(sum);
    free(sqf);

    return result;
}
