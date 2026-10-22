/*
 * A360387  a(1) = 1, and for n > 1, a(n) is the number of ways that a(1..n-1)
 *          can be divided into contiguous subsequences of equal sum.
 * https://oeis.org/A360387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360387
    a(1) = 1.
    For n > 1, a(n) = number of ways that a(1..n‑1) can be divided
    into contiguous subsequences of equal sum.
    The implementation enumerates divisors of the total sum of the
    previous sequence and checks each one by a simple linear scan.
    This runs in roughly O(n²) time and uses only the required
    arithmetic on seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration required by the build. */
seq_t a(idx_t n);

/* Memory for the sequence and its prefix sums.  0‑th entry is unused. */
static seq_t *cache = NULL;   /* a-values   : cache[1..]   */
static seq_t *pref  = NULL;   /* prefix sum : pref[1..]    */
static idx_t  cap   = 0;      /* allocated capacity */
static idx_t  len   = 0;      /* largest n computed */

/* Ensure at least n+1 elements can be stored. */
static void ensure_capacity(idx_t n)
{
    if (!cache || cap < n) {
        idx_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap < n) new_cap <<= 1;          /* double until enough   */
        cache = (seq_t*)realloc(cache, sizeof(seq_t)*(new_cap + 1));
        pref  = (seq_t*)realloc(pref , sizeof(seq_t)*(new_cap + 1));
        cap   = new_cap;
        if (len == 0) { /* initialise the first term lazily later */
            cache[0] = (seq_t)0;
            pref[0]  = (seq_t)0;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* defined for safety */

    ensure_capacity(n);

    /* Initialise when called the first time. */
    if (len == 0) {
        cache[1] = (seq_t)1;
        pref[1]  = cache[1];
        len = 1;
    }

    /* Return already computed value. */
    if (n <= len) return cache[n];

    /* Buffer for divisors – 5000 is ample for 64‑bit values. */
    static uint64_t divisors[5000];
    size_t div_cnt;

    /* Compute missing terms (len+1 .. n). */
    for (idx_t idx = len + 1; idx <= n; ++idx) {
        seq_t total = pref[idx - 1];          /* T = sum of a(1..idx-1) */

        /* Enumerate all divisors of total (using 64‑bit arithmetic). */
        uint64_t t_val = (uint64_t)total;     /* fits for the required limits */
        div_cnt = 0;
        for (uint64_t d = 1; d * d <= t_val; ++d) {
            if (t_val % d == 0) {
                divisors[div_cnt++] = d;
                if (d != t_val / d) divisors[div_cnt++] = t_val / d;
            }
        }

        /* Count valid partitions. */
        seq_t count = (seq_t)0;
        for (size_t k = 0; k < div_cnt; ++k) {
            seq_t block = (seq_t)divisors[k];

            /* target block sum equal to total is always valid – skip scan. */
            if (block == total) {
                ++count;
                continue;
            }

            seq_t sum = (seq_t)0;
            bool ok = true;
            for (idx_t j = 1; j < idx; ++j) {
                sum += cache[j];
                if (sum == block) {
                    sum = (seq_t)0;
                } else if (sum > block) {      /* early failure */
                    ok = false;
                    break;
                }
            }
            if (ok && sum == (seq_t)0) ++count;
        }

        cache[idx] = count;
        pref[idx] = pref[idx - 1] + count;
    }

    len = n;
    return cache[n];
}
