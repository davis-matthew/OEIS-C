/*
 * A393015  Palindromic canyon numbers.
 * https://oeis.org/A393015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 80, u32 n <= 627, u64 n <= 1013,
 *            u128 n <= 1013, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate palindromic canyon numbers by recursively building
 * strictly decreasing digit sequences of odd length greater than one.
 * The sequence is sorted in ascending order by construction. */
#include <stddef.h>

static seq_t cache[1013];
static int initialized = 0;

/* Recursive helper: build all palindromic canyon numbers for a given
 * left‐half length k+1 (k ≥ 1).  'idx' is the current position in the
 * digits array; 'prev' is the maximum digit allowed at this position
 * (strictly decreasing sequence).  'digits' holds the sequence.
 * Increment *cnt for each generated number stored in cache. */
static void rec(int k, int idx, int prev, int digits[], size_t *cnt)
{
    if (idx == k + 1) {
        seq_t val = 0;
        /* left half including the middle digit */
        for (int i = 0; i <= k; ++i)
            val = val * 10 + (seq_t)digits[i];
        /* mirror (exclude middle digit for odd length) */
        for (int i = k - 1; i >= 0; --i)
            val = val * 10 + (seq_t)digits[i];
        cache[(*cnt)++] = val;
        return;
    }

    int start = (idx == 0) ? 1 : 0;   /* leading digit cannot be 0 */
    for (int d = start; d < prev; ++d) {
        digits[idx] = d;
        rec(k, idx + 1, d, digits, cnt);
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        size_t cnt = 0;
        int digits[10];
        /* generate for lengths 3,5,…,19  (k = 1…9) */
        for (int k = 1; k <= 9; ++k)
            rec(k, 0, 10, digits, &cnt);
        /* cnt should now be 1013 */
        initialized = 1;
    }

    if (n == 0 || n > 1013)
        return 0;

    return cache[(idx_t)n - 1];
}
