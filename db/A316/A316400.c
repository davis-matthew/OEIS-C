/*
 * A316400  Number of strict integer partitions of n that are knapsack (every
 *          subset has a different sum) but not every subset has a different
 *          average.
 * https://oeis.org/A316400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict integer partitions of n that are
 *            knapsack * (all subset sums distinct) but not average‑distinct
 *            (some two subsets have equal average). * * This implementation
 *            enumerates all strict partitions that satisfy the * knapsack
 *            condition by maintaining a bitset of reachable subset...
 * Range:     u8 n <= 48, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   add_part, gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/add_part.h"
#include "../utils/gcd_int.h"

/*---------------------------------------*/
/* Configuration constants               */
#define MAX_WORDS   256      /* enough for n up to ~16000 (256*64 = 16384 bits) */
#define MAX_PARTS   32
#define KEY_MAX     400000   /* upper bound for average hash key */

/*---------------------------------------*/
/* Global data used in recursive search */
static int  parts[MAX_PARTS];
static size_t part_depth = 0;
static unsigned char used[KEY_MAX] = {0};

/*---------------------------------------*/
/* Utility functions                     */

/* Euclidean gcd for small integers */

/* Check whether the current partition (stored in `parts` / `part_depth`)
 * has a duplicate average.  Returns true if a duplicate exists.
 */
static bool has_duplicate_average(void)
{
    size_t k = part_depth;
    if (k < 3) return false;   /* cannot have duplicate averages with <3 parts */

    int totalSubs = 1 << (int)k;
    uint32_t used_idx[MAX_PARTS * 8] = {0};   /* enough for up to 2^15 subsets */
    size_t idx_count = 0;

    for (int mask = 1; mask < totalSubs; ++mask) {
        int sum = 0, cnt = 0;
        int m   = mask;
        int ibit = 0;
        while (m) {
            if (m & 1) {
                sum += parts[ibit];
                cnt++;
            }
            ibit++;
            m >>= 1;
        }
        int g = gcd_int(sum, cnt);
        uint32_t key = ((uint32_t)(cnt / g)) * 10001U + (uint32_t)(sum / g);

        if (used[key]) {
            /* clear flag table before returning */
            for (size_t j = 0; j < idx_count; ++j) used[used_idx[j]] = 0;
            return true;
        }
        used[key] = 1;
        used_idx[idx_count++] = key;
    }

    /* clear flags for next use */
    for (size_t j = 0; j < idx_count; ++j) used[used_idx[j]] = 0;
    return false;
}

/* Recursive search for all knapsack partitions of remaining sum `rem`.
 * `last`   : last chosen part (ensures strictly increasing parts)
 * `bits`   : bitset of subset sums achievable with current parts
 * `words`  : number of 64‑bit words in the bitset
 * `ans`    : pointer to running count (seq_t)
 */
static void dfs(idx_t rem,
                int last,
                uint64_t bits[],
                size_t words,
                seq_t *ans)
{
    if (rem == 0) {
        if (has_duplicate_average())
            (*ans) += (seq_t)1;
        return;
    }

    int start = last + 1;                     /* next part must be > last */
    for (int p = start; p <= (int)rem; ++p) {

        /* prune impossible branches: we need at least one more part > p
           when remaining > 0 */
        if (rem - p > 0 && (rem - p) < p + 1)  continue;

        uint64_t newbits[MAX_WORDS];
        if (!add_part(bits, newbits, words, p))
            continue;

        parts[part_depth++] = p;
        dfs(rem - p, p, newbits, words, ans);
        --part_depth;
    }
}

/*---------------------------------------*/
/* Public API function                    */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    size_t words = ( (size_t)n + 1 + 63 ) / 64;   /* bits for sums 0..n inclusive */

    uint64_t bits[MAX_WORDS] = {0};
    bits[0] = 1ULL;       /* sum 0 is always reachable */

    part_depth = 0;
    seq_t answer = 0;

    dfs((idx_t)n, 0, bits, words, &answer);
    return answer;
}
