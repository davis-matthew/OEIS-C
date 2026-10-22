/*
 * A359515  Number of compositions (ordered partitions) of n into at most 3
 *          positive Fibonacci numbers (with a single type of 1).
 * https://oeis.org/A359515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of compositions of n into at most 3 positive *
 *            Fibonacci numbers (with the Fibonacci set {1,2,3,5,8,13,…}, i.e.
 *            1 occurs * only once). The implementation uses dynamic
 *            programming over the * number of parts (0‑3). Fibonacci numbers
 *            are generated lazily up to the * required index. The...
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* static storage for Fibonacci numbers up to the current limit */
static idx_t   *fibs      = NULL;   /* Fibonacci numbers ≤ current limit */
static size_t  fib_cap   = 0;        /* allocated capacity of fibs       */
static size_t  fib_cnt   = 0;        /* number of Fibonacci numbers stored */

/* dynamic programming tables:
 * dp[k][i] = #compositions of i with exactly k parts (k = 0…3)      */
static seq_t  *dp[4]     = { NULL, NULL, NULL, NULL };
static size_t  dp_cap    = 0;        /* current allocated capacity for dp tables   */
static idx_t   computed_up_to = 0;   /* first index not yet computed (exclusive) */

/* ------------------------------------------------------------------ */
/* Ensure that fibs contains all Fibonacci numbers ≤ limit             */
static void
ensure_fibs(idx_t limit)
{
    /* allocate initially if needed */
    if (fibs == NULL) {
        fib_cap = 4;
        fibs = (idx_t *)malloc(fib_cap * sizeof(idx_t));
        fib_cnt = 0;
    }

    /* start with the first Fibonacci number 1 */
    if (fib_cnt == 0) {
        if (fib_cnt == fib_cap) {
            fib_cap <<= 1;
            fibs = (idx_t *)realloc(fibs, fib_cap * sizeof(idx_t));
        }
        fibs[fib_cnt++] = 1ULL;
    }

    /* add the second Fibonacci number 2 if required */
    if (fib_cnt == 1 && limit >= 2) {
        if (fib_cnt == fib_cap) {
            fib_cap <<= 1;
            fibs = (idx_t *)realloc(fibs, fib_cap * sizeof(idx_t));
        }
        fibs[fib_cnt++] = 2ULL;
    }

    /* generate further Fibonacci numbers */
    while (fib_cnt >= 2) {
        idx_t next = fibs[fib_cnt-1] + fibs[fib_cnt-2];
        if (next > limit) break;
        if (fib_cnt == fib_cap) {
            fib_cap <<= 1;
            fibs = (idx_t *)realloc(fibs, fib_cap * sizeof(idx_t));
        }
        fibs[fib_cnt++] = next;
    }
}

/* ------------------------------------------------------------------ */
/* Expand the DP tables so that indices 0 … limit are valid            */
static void
expand_dp_if_needed(idx_t limit)
{
    if ((size_t)(limit + 1) <= dp_cap)
        return;

    size_t new_cap = 1;
    while (new_cap < (size_t)(limit + 1))
        new_cap <<= 1;

    for (int k = 0; k < 4; ++k) {
        if (dp[k] == NULL) {
            dp[k] = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            seq_t *tmp = (seq_t *)realloc(dp[k], new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(EXIT_FAILURE);
            /* zero the newly allocated part */
            memset(tmp + dp_cap, 0, (new_cap - dp_cap) * sizeof(seq_t));
            dp[k] = tmp;
        }
    }
    dp_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main entry function                                                 */
seq_t
a(idx_t n)
{
    /* 1. prepare Fibonacci numbers up to n */
    ensure_fibs(n);

    /* 2. make sure DP tables can address 0 … n */
    expand_dp_if_needed(n);

    /* 3. initialise base case for zero parts: only one empty composition */
    if (dp[0][0] == 0)
        dp[0][0] = (seq_t)1;

    /* 4. compute DP values for all new indices up to n */
    if (computed_up_to <= n) {
        for (idx_t cur = computed_up_to; cur <= n; ++cur) {
            /* dp[1] – exactly one part (any Fibonacci that equals cur) */
            seq_t sum1 = 0;
            for (size_t fi = 0; fi < fib_cnt && fibs[fi] <= cur; ++fi) {
                idx_t f = fibs[fi];
                sum1 += dp[0][(size_t)(cur - f)];
            }
            dp[1][(size_t)cur] = sum1;

            /* dp[2] – exactly two parts */
            seq_t sum2 = 0;
            for (size_t fi = 0; fi < fib_cnt && fibs[fi] <= cur; ++fi) {
                idx_t f = fibs[fi];
                sum2 += dp[1][(size_t)(cur - f)];
            }
            dp[2][(size_t)cur] = sum2;

            /* dp[3] – exactly three parts */
            seq_t sum3 = 0;
            for (size_t fi = 0; fi < fib_cnt && fibs[fi] <= cur; ++fi) {
                idx_t f = fibs[fi];
                sum3 += dp[2][(size_t)(cur - f)];
            }
            dp[3][(size_t)cur] = sum3;
        }
        computed_up_to = n + 1;
    }

    /* 5. result is the sum of DP values for 0,1,2,3 parts */
    seq_t result = 0;
    result += dp[0][(size_t)n];
    result += dp[1][(size_t)n];
    result += dp[2][(size_t)n];
    result += dp[3][(size_t)n];
    return result;
}
