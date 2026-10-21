/*
 * A272029  Least number k > n such that the multiplicative and additive
 *          digital roots of k are both equal to n.
 * https://oeis.org/A272029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   add_root_uint64, build_min_value, mult_root_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/add_root_uint64.h"
#include "../utils/build_min_value.h"
#include "../utils/mult_root_uint64.h"

/* This function implements the definition of OEIS sequence A272029:
   a(n) = least k > n such that the additive and multiplicative
   digital roots of k are both equal to n.  The sequence is defined
   only for n = 1..9 (higher questions have no solution).  The
   implementation enumerates all decimal digit multisets (no zeros)
   up to a modest length, computes the digital roots of each multiset,
   and keeps the smallest number whose roots equal the target digit. */

static const int MAX_LEN = 12;                    /* enough for all cases */
static uint64_t results[10];                     /* indices 1..9 used */
static bool initialized = false;
static int cnt[10];                               /* digit counts 0..9 */

/* Process a complete digit multiset after enumeration. */
static void process_combination(void)
{
    uint64_t sum = 0, prod = 1;
    int len = 0;
    for (int d = 1; d <= 9; ++d) {
        int c = cnt[d];
        len += c;
        sum += (uint64_t)d * (uint64_t)c;
        for (int i = 0; i < c; ++i)
            prod *= (uint64_t)d;
    }
    int root = add_root_uint64(sum);
    if (root < 1 || root > 9) return;
    int mroot = mult_root_uint64(prod);
    if (mroot != root) return;
    uint64_t val = build_min_value(cnt, len);
    if (val <= (uint64_t)root) return;        /* must be strictly greater than n */
    if (val < results[root])
        results[root] = val;
}

/* Recursive enumeration of digit counts that sum to 'remaining'. */
static void enum_counts(int digit, int remaining)
{
    if (digit == 10) {
        if (remaining != 0) return;
        process_combination();
        return;
    }
    for (int c = 0; c <= remaining; ++c) {
        cnt[digit] = c;
        enum_counts(digit + 1, remaining - c);
    }
}

/* One‑time initialization that fills the results table. */
static void init_results(void)
{
    for (int i = 0; i <= 9; ++i)
        results[i] = UINT64_MAX;
    for (int len = 1; len <= MAX_LEN; ++len)
        enum_counts(1, len);
    initialized = true;
}

/* ------------------------------------------------------------------- */
/* The function required by the build system */
seq_t a(idx_t n)
{
    if (n > (idx_t)9) return (seq_t)0;
    if (!initialized) init_results();
    return (seq_t)results[(int)n];
}
