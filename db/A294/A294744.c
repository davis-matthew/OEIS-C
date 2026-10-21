/*
 * A294744  Numbers that are the sum of 5 nonzero squares in exactly 10 ways.
 * https://oeis.org/A294744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are the sum of 5 nonzero squares in exactly 10 unordered ways */
/* This function pre‑computes all such numbers up to the proven upper bound 7845
   and returns the n‑th term (1‑based index).  The list is finite, so after the
   last term the function returns 0. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Max value known to contain all solutions (proved by the linked theorem). */
#define MAX_N 7845

/* Internal storage for computed solutions.  Capacity is sufficiently large
   for the very short list of solutions (11 terms). */
static bool _initialized = false;
static seq_t _sols[20];
static size_t _count = 0;

/* Pre‑compute the solution list once. */
static void _precompute(void)
{
    /* dp[k][s] = number of unordered representations of s as a sum of
       exactly k squares (using only positive squares).  The array is
       static so it is zero‑initialized by the C runtime. */
    static uint32_t dp[6][MAX_N + 1] = {0};

    dp[0][0] = 1;  /* one way to get 0 with 0 squares */

    for (int i = 1; (int)i * i <= MAX_N; ++i) {
        int sq = i * i;
        for (int sum = 0; sum + sq <= MAX_N; ++sum) {
            for (int k = 0; k < 5; ++k) {
                uint32_t v = dp[k][sum];
                if (v != 0) {
                    dp[k + 1][sum + sq] += v;
                }
            }
        }
    }

    /* Collect all numbers with exactly 10 unordered representations as 5 squares. */
    for (int n = 1; n <= MAX_N; ++n) {
        if (dp[5][n] == 10) {
            if (_count < sizeof(_sols) / sizeof(_sols[0])) {
                _sols[_count++] = (seq_t)n;
            }
        }
    }
}

/* Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (!_initialized) {
        _precompute();
        _initialized = true;
    }

    if (n == 0) return 0;                     /* offset is 1 */
    size_t idx = (size_t)(n - 1);              /* 0‑based index */
    if (idx < _count) return _sols[idx];
    return 0;                                 /* no further terms */
}
