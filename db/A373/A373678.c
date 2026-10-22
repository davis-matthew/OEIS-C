/*
 * A373678  Sums of maximal runs of non-prime-powers.
 * https://oeis.org/A373678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A373678: Sums of maximal runs of non-prime-powers
 *
 * The run is taken from the sequence of natural numbers (starting at 1),
 * where a number is considered a non‑prime‑power if it is NOT a power of a
 * single prime (primes themselves are prime‑powers). 1 is treated as a
 * non‑prime‑power, but *not* as a prime‑power.
 * Each maximal interval of consecutive non‑prime‑powers is summed; the
 * resulting sum is the term a(n).  Example: 1 → sum 1, 6 → sum 6, 10 → sum 10,
 * 12 → sum 12, 14–15 → sum 29, …
 *
 * Implementation note: Uses an internal cache of computed run sums so that
 * multiple calls to a(idx_t n) are efficient.  Prime‑power determination
 * is done by simple trial division, which is fast enough for the index
 * ranges required by the test harness (up to a few thousand terms).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Check whether m (>1) is a prime‑power (i.e., m = p^k, p prime, k≥1). */
static bool is_prime_power_uint64(uint64_t m)
{
    if (m <= 1) return false;           /* 1 is NOT a prime‑power by definition */
    uint64_t p = 0;
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            p = i;
            break;
        }
    }
    if (p == 0) return true;            /* m itself is prime */
    /* Divide by p repeatedly; if we end with 1, m is a power of p. */
    uint64_t t = m;
    while (t % p == 0) t /= p;
    return t == 1;
}

/* Main sequence function.  idx_t is the index type, seq_t the output type. */
seq_t a(idx_t n)
{
    /* Persistent static state for memoisation */
    static seq_t *run_sums = NULL;
    static size_t run_cnt   = 0;
    static size_t run_cap   = 0;

    static uint64_t cur_val  = 1;          /* next integer to examine */
    static bool    in_run   = false;       /* inside a run of non‑prime‑powers? */
    static seq_t  cur_sum   = 0;           /* current run's running total */

    /* Initial allocation */
    if (run_sums == NULL) {
        run_cap   = 128;
        run_sums  = (seq_t *)malloc(run_cap * sizeof(seq_t));
    }

    /* Expand run array if needed */
    if (run_cnt >= n && run_sums != NULL) {
        /* We already have the desired term cached */
        return run_sums[(size_t)(n - 1)];
    }

    /* Generate runs until we have at least n runs stored */
    while (run_cnt < n) {
        bool non_prime = (cur_val == 1) || !is_prime_power_uint64(cur_val);

        if (non_prime) {                     /* part of a run */
            if (!in_run) {                   /* start a new run */
                in_run = true;
                cur_sum = (seq_t)cur_val;
            } else {                         /* extend current run */
                cur_sum += (seq_t)cur_val;
            }
        } else {                             /* prime‑power → run ends */
            if (in_run) {
                /* store current run's sum */
                if (run_cnt == run_cap) {
                    /* grow capacity */
                    size_t new_cap = run_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(run_sums,
                                                  new_cap * sizeof(seq_t));
                    if (tmp == NULL) exit(EXIT_FAILURE); /* memory failure */
                    run_sums = tmp;
                    run_cap  = new_cap;
                }
                run_sums[run_cnt++] = cur_sum;
                in_run  = false;
                cur_sum = 0;
            }
        }
        ++cur_val;
    }

    return run_sums[(size_t)(n - 1)];
}
