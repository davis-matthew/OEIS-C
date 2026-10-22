/*
 * A348380  Number of factorizations of n without an alternating permutation.
 *          Includes all twins (x*x).
 * https://oeis.org/A348380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of factorizations of n without an alternating
 *            permutation. This implementation enumerates all multiplicative
 *            partitions of n (weakly increasing factors > 1) and counts those
 *            for which no rearrangement of the factors yields a strict
 *            alternating sequence. The algorithm uses a backtracking se...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only standard C headers.  */
#include <stdint.h>
#include <stdbool.h>

/* Helper: backtracking search for an alternating permutation.  */
static bool dfs_alt(int rem[], const int vals[], int vcnt, int pos,
                    int last_val, bool start_up, int total)
{
    if (pos == total) return true;        /* all elements used */

    bool must_greater;
    if (start_up)          /* pattern: < > < > ...   */
        must_greater = (pos % 2 == 1);     /* next > last if pos odd  */
    else                   /* pattern: > < > < ...   */
        must_greater = !(pos % 2 == 1);    /* next < last if pos odd  */

    for (int j = 0; j < vcnt; ++j) {
        if (rem[j] == 0) continue;
        int val = vals[j];
        if (val == last_val) continue;     /* strict inequality required */
        if (must_greater) {
            if (val <= last_val) continue;
        } else {
            if (val >= last_val) continue;
        }
        rem[j]--;
        if (dfs_alt(rem, vals, vcnt, pos + 1, val, start_up, total)) {
            rem[j]++;
            return true;
        }
        rem[j]++;
    }
    return false;
}

/* Does the multiset of values (vals[0..vcnt-1] with counts cnt[0..vcnt-1])
   admit an alternating permutation? */
static bool has_alternating(const int vals[], const int cnt[],
                            int vcnt, int total)
{
    int rem[64];
    for (int i = 0; i < vcnt; ++i) rem[i] = cnt[i];

    for (int pat = 0; pat < 2; ++pat) {   /* try both starting patterns */
        bool start_up = (pat == 0);
        for (int j = 0; j < vcnt; ++j) {
            if (rem[j] == 0) continue;
            rem[j]--;
            if (dfs_alt(rem, vals, vcnt, 1, vals[j], start_up, total)) {
                rem[j]++;
                return true;
            }
            rem[j]++;
        }
    }
    return false;
}

/* Process a single factorization stored in cur[0..depth-1] and update *ans. */
static void process(int depth, const uint64_t cur[], seq_t *ans)
{
    if (depth < 2) return;   /* at least two parts needed */

    if (depth == 2) {                    /* only two parts: special case */
        if (cur[0] == cur[1]) (*ans)++;
        return;
    }

    /* Build list of distinct values and their multiplicities. */
    int vals[64];
    int cnt[64];
    int vcnt = 0;
    for (int i = 0; i < depth; ++i) {
        if (vcnt == 0 || cur[i] != vals[vcnt - 1]) {
            vals[vcnt] = (int)cur[i];
            cnt[vcnt] = 1;
            vcnt++;
        } else {
            cnt[vcnt - 1]++;
        }
    }

    /* Quick rejection: a value that occurs too often cannot be separated. */
    int thresh = (depth + 1) / 2;        /* floor((k+1)/2) */
    for (int i = 0; i < vcnt; ++i)
        if (cnt[i] > thresh) {
            (*ans)++;
            return;
        }

    /* Count total parts (should equal depth). */
    int total = 0;
    for (int i = 0; i < vcnt; ++i) total += cnt[i];

    /* Check whether an alternating permutation exists. */
    if (!has_alternating(vals, cnt, vcnt, total))
        (*ans)++;
}

/* Recursive enumeration of multiplicative partitions. */
static void gen(uint64_t rem, uint64_t min_factor,
                uint64_t cur[], int depth, seq_t *ans)
{
    if (rem == 1) {                 /* finished a partition */
        process(depth, cur, ans);
        return;
    }

    for (uint64_t f = min_factor; f * f <= rem; ++f) {
        if (rem % f == 0) {
            cur[depth] = f;
            gen(rem / f, f, cur, depth + 1, ans);
        }
    }
    /* Add the final factor rem if it respects the non‑decreasing order. */
    if (rem >= min_factor && rem > 1) {
        cur[depth] = rem;
        process(depth + 1, cur, ans);
    }
}

/* The required function: a(n) */
seq_t a(idx_t n)
{
    seq_t ans = 0;
    uint64_t cur[64];
    gen((uint64_t)n, 2, cur, 0, &ans);
    return ans;
}
