/*
 * A295916  Number of nX6 0..1 arrays with each 1 adjacent to 0 or 3 king-move
 *          neighboring 1s.
 * https://oeis.org/A295916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 0/1 arrays with each 1 adjacent to 0 or 3
 *            king‑move neighboring 1s. (No simple closed form.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts n×6 binary arrays in which each 1 has either
   0 or 3 king‑move adjacent 1s.  A dynamic‑programming algorithm
   that keeps the last two rows is used.  No lookup table is stored;
   results are memoised so that calls are fast in any order. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Transition mask: trans_mask[p0][p1] has bit cur set iff
   the row p1 is valid when surrounded by rows p0 (above) and
   p2 = cur (below). */
static uint64_t trans_mask[64][64];

/* Checks if mask p1 is valid given surrounding rows p0 and p2. */
static bool row_valid(int p0, int p1, int p2)
{
    for (int j = 0; j < 6; ++j) {
        if (!(p1 >> j & 1))
            continue;
        int cnt = 0;
        /* above row */
        if (j > 0   && (p0 >> (j-1) & 1)) ++cnt;
        if (p0 >> j         & 1)          ++cnt;
        if (j < 5   && (p0 >> (j+1) & 1)) ++cnt;
        /* current row left/right */
        if (j > 0   && (p1 >> (j-1) & 1)) ++cnt;
        if (j < 5   && (p1 >> (j+1) & 1)) ++cnt;
        /* below row */
        if (j > 0   && (p2 >> (j-1) & 1)) ++cnt;
        if (p2 >> j         & 1)          ++cnt;
        if (j < 5   && (p2 >> (j+1) & 1)) ++cnt;
        if (!(cnt == 0 || cnt == 3))
            return false;
    }
    return true;
}

/* Precompute transition masks once. */
static void init_transition_masks(void)
{
    for (int p0 = 0; p0 < 64; ++p0)
        for (int p1 = 0; p1 < 64; ++p1) {
            uint64_t mask = 0;
            for (int p2 = 0; p2 < 64; ++p2)
                if (row_valid(p0, p1, p2))
                    mask |= 1ULL << p2;
            trans_mask[p0][p1] = mask;
        }
}

/* DP state: dp[p0][p1] = number of ways after the last two rows
   are p0 (row r‑1) and p1 (row r). */
static seq_t dp[64][64];
static seq_t newdp[64][64];

/* Static state for memoisation and storage of results. */
static bool initialized = false;
static idx_t current_n = 0;                /* last n processed in DP */
static idx_t max_computed = 0;             /* highest index of stored value */

#ifdef __cplusplus
static std::vector<seq_t> sequence;
#else
static seq_t *sequence = NULL;
static idx_t seq_len = 0;                  /* allocated length */
#endif

static void ensure_storage(idx_t n)
{
#ifdef __cplusplus
    if (sequence.size() <= n)
        sequence.resize(n + 1);            /* default‑constructed seq_t */
#else
    if (n + 1 > seq_len) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(sequence, new_size * sizeof(seq_t));
        if (!tmp) abort();
        sequence = tmp;
        /* zero any newly added elements (trivial type). */
        if (new_size > seq_len)
            memset(sequence + seq_len, 0, (new_size - seq_len) * sizeof(seq_t));
        seq_len = new_size;
    }
#endif
}

/* The main function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_transition_masks();
        initialized = true;
    }

    ensure_storage(n);

    if (n <= max_computed) {                /* already computed */
#ifdef __cplusplus
        return sequence[(size_t)n];
#else
        return sequence[n];
#endif
    }

    /* initialise DP for the very first row if we have no DP data yet */
    if (current_n == 0) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = 0;
        for (int mask = 0; mask < 64; ++mask)
            dp[0][mask] = 1;                /* state after first row */

        /* compute a(1) */
#ifdef __cplusplus
        if (sequence.size() <= 1) sequence.resize(2);
#endif
        seq_t ans = 0;
        for (int mask = 0; mask < 64; ++mask)
            if (trans_mask[0][mask] & 1ULL)
                ++ans;
#ifdef __cplusplus
        sequence[1] = ans;
#else
        sequence[1] = ans;
#endif
        current_n = 1;
        max_computed = 1;
        if (n == 1) return ans;
    }

    /* Extend DP up to the requested n. */
    for (idx_t i = current_n + 1; i <= n; ++i) {
        /* transition: add a new row */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                newdp[a][b] = 0;
        for (int p0 = 0; p0 < 64; ++p0)
            for (int p1 = 0; p1 < 64; ++p1) {
                seq_t val = dp[p0][p1];
                if (!val) continue;
                uint64_t mask = trans_mask[p0][p1];
                while (mask) {
                    int cur = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    newdp[p1][cur] += val;
                }
            }
        /* copy newdp into dp */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                dp[a][b] = newdp[a][b];

        /* compute answer for current i by assuming a trailing zero row */
        seq_t ans = 0;
        for (int p0 = 0; p0 < 64; ++p0)
            for (int p1 = 0; p1 < 64; ++p1) {
                seq_t val = dp[p0][p1];
                if (!val) continue;
                if (trans_mask[p0][p1] & 1ULL)
                    ans += val;
            }
#ifdef __cplusplus
        if (sequence.size() <= i) sequence.resize(i + 1);
        sequence[(size_t)i] = ans;
#else
        sequence[i] = ans;
#endif
    }
    current_n = n;
    max_computed = n;

    return
#ifdef __cplusplus
        sequence[(size_t)n];
#else
        sequence[n];
#endif
}
