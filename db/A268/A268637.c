/*
 * A268637  Number of nX6 0..2 arrays with some element plus some horizontally
 *          or vertically adjacent neighbor totalling two exactly once.
 * https://oeis.org/A268637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×6 arrays over {0,1,2} with exactly one pair of
 *            horizontally * or vertically adjacent cells summing to 2. *
 *            Computed by dynamic programming for n ≤ 15 and by the following
 *            14‑term * linear recurrence for n > 15: * * a(n) = 60*a(n-1)
 *            -1482*a(n-2) +20016*a(n-3) -167163*a(n-4) +924894*a(n...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const int coeffs[14] = {
    60, -1482, 20016, -167163, 924894,
    -3537110, 9593166, -18693789, 26218062,
    -26188161, 18139608, -8262712, 2220864, -266256
};

static seq_t *seq_cache = NULL;          /* 1‑based cache of a(n) */
static size_t seq_cap = 0;               /* allocated size of cache */
static bool first15_done = false;        /* whether the first 15 values are computed */

/* Precomputed data for DP up to n = 15 */
static int8_t H[729];                     /* horizontal pair count for each row */
static uint8_t V[729][729];              /* vertical pair count between two rows */

/* Helper: allocate or enlarge cache */
static void ensure_cache(idx_t n)
{
    if (seq_cap >= (size_t)n + 1) return;
    size_t new_cap = n + 1;
    if (new_cap < 16) new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    for (size_t i = seq_cap; i < new_cap; ++i) seq_cache[i] = 0;
    seq_cap = new_cap;
}

/* Build H and V tables. This is done once lazily. */
static void build_tables(void)
{
    /* Precompute H: horizontal pair count for each 6‑digit base‑3 row */
    for (int idx = 0; idx < 729; ++idx) {
        int v = idx;
        int hcnt = 0;
        int prev = -1;
        for (int pos = 0; pos < 6; ++pos) {
            int digit = v % 3;
            v /= 3;
            if (pos > 0) {
                if ((prev + digit) == 2) hcnt++;
            }
            prev = digit;
        }
        H[idx] = (int8_t)hcnt;
    }
    /* Precompute V: vertical pair count between two rows */
    for (int i = 0; i < 729; ++i) {
        for (int j = 0; j < 729; ++j) {
            int vi = i, vj = j;
            int vcnt = 0;
            for (int pos = 0; pos < 6; ++pos) {
                if (((vi % 3) + (vj % 3)) == 2) vcnt++;
                vi /= 3;
                vj /= 3;
            }
            V[i][j] = (uint8_t)vcnt;
        }
    }
}

/* Compute the first 15 terms using DP and store them in seq_cache */
static void compute_first15(void)
{
    build_tables();

    static seq_t dp0[729], dp1[729];
    static seq_t newdp0[729], newdp1[729];

    /* n = 1 */
    for (int i = 0; i < 729; ++i) {
        if (H[i] == 0) dp0[i] = (seq_t)1;
        else if (H[i] == 1) dp1[i] = (seq_t)1;
    }
    seq_cache[1] = (seq_t)0;
    for (int i = 0; i < 729; ++i) seq_cache[1] += dp1[i];

    /* n = 2 .. 15 */
    for (int n = 2; n <= 15; ++n) {
        for (int i = 0; i < 729; ++i) {
            newdp0[i] = 0;
            newdp1[i] = 0;
        }
        for (int pr = 0; pr < 729; ++pr) {
            seq_t a0 = dp0[pr];
            seq_t a1 = dp1[pr];
            if (a0 == 0 && a1 == 0) continue;
            for (int cur = 0; cur < 729; ++cur) {
                int add = H[cur] + V[pr][cur];
                if (add == 0) {
                    newdp0[cur] += a0;
                    newdp1[cur] += a1;
                } else if (add == 1) {
                    newdp1[cur] += a0;
                }
            }
        }
        seq_cache[n] = 0;
        for (int i = 0; i < 729; ++i) seq_cache[n] += newdp1[i];
        for (int i = 0; i < 729; ++i) {
            dp0[i] = newdp0[i];
            dp1[i] = newdp1[i];
        }
    }
    first15_done = true;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    if (!first15_done) compute_first15();
    if (seq_cache[n] != 0 || n == 1) return seq_cache[n];

    /* n > 15 and not yet computed */
    for (idx_t k = 16; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int j = 0; j < 14; ++j) {
            seq_t val = seq_cache[k - (idx_t)(j + 1)];
            int coeff = coeffs[j];
            if (coeff >= 0) {
                pos += (seq_t)coeff * val;
            } else {
                neg += (seq_t)(-coeff) * val;
            }
        }
        seq_cache[k] = pos - neg;   /* pos >= neg for valid terms */
    }
    return seq_cache[n];
}
