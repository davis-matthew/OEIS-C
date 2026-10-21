/*
 * A253325  Number of (n+1)X(7+1) 0..1 arrays with every 2X2 subblock sum
 *          nondecreasing horizontally, vertically and antidiagonally
 *          ne-to-sw.
 * https://oeis.org/A253325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×8 0/1 arrays with every 2×2 subblock sum
 *            * nondecreasing horizontally, vertically and anti‑diagonally
 *            (NE→SW). * For n>26: a(n)=4 a(n−1)+2 a(n−2)−26 a(n−3)+16
 *            a(n−4)+62 a(n−5) * −70 a(n−6)−62 a(n−7)+109 a(n−8)+14 a(n−9)−76
 *            a(n−10) * +16 a(n−11)+20 a(n−12)−8 a(n−13)
 * Range:     u8 none, u16 n <= 2, u32 n <= 19, u64 n <= 51, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* coefficients of the 13‑term recurrence, indexed 0..12 for k=1..13 */
static const int coeffs[13] = { 4, 2, -26, 16, 62, -70, -62, 109, 14, -76, 16, 20, -8 };

/* table of 2×2 block sums for all 256×256 pairs of rows, 7 blocks per pair */
static uint8_t blockSum[256][256][7];
static int blockSum_ready = 0;

/* compute the table only once */
static void compute_block_sums(void)
{
    if (blockSum_ready) return;
    for (int a = 0; a < 256; ++a) {
        for (int b = 0; b < 256; ++b) {
            for (int j = 0; j < 7; ++j) {
                uint8_t s = ((a >> j) & 1) + ((a >> (j + 1)) & 1)
                          + ((b >> j) & 1) + ((b >> (j + 1)) & 1);
                blockSum[a][b][j] = s;
            }
        }
    }
    blockSum_ready = 1;
}

/* compute first 26 terms by dynamic programming */
static void compute_first_26_terms(seq_t *base)
{
    compute_block_sums();

    const size_t STATES = 256U * 256U;
    seq_t *dp      = (seq_t *)malloc(STATES * sizeof(seq_t));
    seq_t *dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));
    if (!dp || !dp_next) return; /* allocation failed */

    memset(dp, 0, STATES * sizeof(seq_t));

    /* initial states for a(1): only horizontal monotonicity */
    for (int prev = 0; prev < 256; ++prev) {
        for (int cur = 0; cur < 256; ++cur) {
            int i = prev * 256 + cur;
            int ok = 1;
            for (int j = 0; j < 6; ++j) /* 6 comparisons for 7 blocks */
                if (blockSum[prev][cur][j] > blockSum[prev][cur][j + 1]) { ok = 0; break; }
            if (ok) dp[i] = 1;
        }
    }

    /* a(1) */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += dp[i];
    base[1] = total;

    /* iterative steps 2 … 26 */
    for (int step = 2; step <= 26; ++step) {
        memset(dp_next, 0, STATES * sizeof(seq_t));
        for (int prev = 0; prev < 256; ++prev) {
            for (int cur = 0; cur < 256; ++cur) {
                seq_t curcnt = dp[prev * 256 + cur];
                if (curcnt == 0) continue;
                for (int next = 0; next < 256; ++next) {
                    /* horizontal monotonicity of (cur,next) */
                    int ok = 1;
                    for (int j = 0; j < 6; ++j)
                        if (blockSum[cur][next][j] > blockSum[cur][next][j + 1]) { ok = 0; break; }
                    if (!ok) continue;
                    /* vertical constraint with (prev,cur) and (cur,next) */
                    for (int j = 0; j < 7; ++j)
                        if (blockSum[prev][cur][j] > blockSum[cur][next][j]) { ok = 0; break; }
                    if (!ok) continue;
                    /* diagonal constraint */
                    for (int j = 0; j < 6; ++j)
                        if (blockSum[prev][cur][j + 1] > blockSum[cur][next][j]) { ok = 0; break; }
                    if (!ok) continue;
                    dp_next[cur * 256 + next] += curcnt;
                }
            }
        }
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;

        /* compute total for this step */
        total = 0;
        for (size_t i = 0; i < STATES; ++i) total += dp[i];
        base[step] = total;
    }

    free(dp);
    free(dp_next);
}

/* Memoized sequence values */
static seq_t *memo = NULL;          /* 0‑based array, index 0 unused */
static size_t memo_n = 0;          /* largest n computed so far */
static int base_computed = 0;      /* whether first 26 terms are ready */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise first 26 values once */
    if (!base_computed) {
        static seq_t base[27];          /* indices 1..26 used */
        compute_first_26_terms(base);

        memo = (seq_t *)malloc(27 * sizeof(seq_t));
        if (!memo) return 0;            /* out of memory, unlikely */
        memo[0] = 0;
        for (int i = 1; i <= 26; ++i) memo[i] = base[i];
        memo_n = 26;
        base_computed = 1;
    }

    if (n <= memo_n) return memo[n];

    /* extend memo up to n using the recurrence */
    memo = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
    for (size_t i = memo_n + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 13; ++k) {
            int c = coeffs[k];
            seq_t prod = memo[i - k - 1] * (uint64_t)(c > 0 ? c : -c);
            if (c > 0) pos += prod; else neg += prod;
        }
        memo[i] = pos - neg;           /* pos ≥ neg for all n */
    }
    memo_n = n;
    return memo[n];
}
