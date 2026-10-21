/*
 * A254123  Least number of terms needed to build n from Fibonacci numbers
 *          using + and *.
 * https://oeis.org/A254123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254123: Least number of Fibonacci terms needed to build n
 *  using + and *.
 *
 *  The implementation uses dynamic programming with memoisation.
 *  It grows its internal cache on demand, so repeated calls in
 *  arbitrary order are handled correctly.
 *  fibs[] is kept up to date to allow quick Fibonacci membership test.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Build Fibonacci numbers up to limit. Maximum number of Fibonacci
 * numbers that fit in an uint64_t is 93 (F93 > 2^63).  We allocate a
 * slightly larger array to be safe. */
#define MAX_FIBS 93

static idx_t fibs[MAX_FIBS];
static idx_t fib_cnt = 0;

/* Extend fibs[] to contain all Fibonacci numbers <= limit. */
static void build_fibs_until(idx_t limit)
{
    if (fib_cnt == 0) {
        fibs[0] = 1;
        fibs[1] = 1;
        fib_cnt = 2;
    }
    while (fib_cnt < MAX_FIBS && fibs[fib_cnt - 1] < limit) {
        fibs[fib_cnt] = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        ++fib_cnt;
    }
}

/* Check whether a value is a Fibonacci number. */
static bool is_fib(idx_t v)
{
    for (idx_t i = 0; i < fib_cnt; ++i) {
        if (fibs[i] == v)
            return true;
        if (fibs[i] > v)
            break;
    }
    return false;
}

/* Dynamic programming cache for a(n).  dp_cache[0] is unused,
 * dp_cache[1] = 1, and dp_cache[i] contains the answer for i. */
static seq_t *dp_cache = NULL;
static idx_t dp_max = 0;

seq_t a(idx_t n)
{
    if (n == 0)      /* not defined in OEIS; return 0 safely */
        return 0;

    /* Grow cache if needed. */
    if (n > dp_max) {
        /* Allocate or extend memory.  The cast is required for C++ */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp_cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure → safe value */
        dp_cache = tmp;

        /* Ensure Fibonacci table covers all values up to n. */
        build_fibs_until(n);

        /* Initialise base case if first time. */
        if (dp_max == 0) {
            dp_cache[0] = 0;
            dp_cache[1] = (seq_t)1;
            dp_max   = 1;
        }

        /* Compute answers for indices > dp_max up to n. */
        for (idx_t val = dp_max + 1; val <= n; ++val) {
            if (is_fib(val)) {
                dp_cache[val] = (seq_t)1;   /* a Fibonacci number itself */
                continue;
            }

            /* Start with a sum split of 1+(val-1) as a safe base candidate. */
            seq_t best = dp_cache[1] + dp_cache[val - 1];

            /* Try all smaller addends a + b with a <= b. */
            for (idx_t a = 1; a <= val / 2; ++a) {
                idx_t b = val - a;
                seq_t cand = dp_cache[a] + dp_cache[b];
                if (cand < best)
                    best = cand;
            }

            /* Try multiplicative splits a * b = val. */
            for (idx_t a = 2; a <= val / a; ++a) {  /* safe bound to avoid overflow */
                if (val % a == 0) {
                    idx_t b = val / a;
                    seq_t cand = dp_cache[a] + dp_cache[b];
                    if (cand < best)
                        best = cand;
                }
            }
            dp_cache[val] = best;
        }
        dp_max = n;
    }
    return dp_cache[n];
}
