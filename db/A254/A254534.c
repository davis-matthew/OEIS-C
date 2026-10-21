/*
 * A254534  Number of length n 1..(3+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A254534
 * Formula: a(n) = number of length‑n arrays with entries in {1,2,3,4,5}
 *              such that no non‑empty prefix sum is a prime.
 *
 * The implementation uses dynamic programming over possible prefix sums.
 * A sieve of Eratosthenes is kept in static cache and extended on demand.
 * It works for any seq_t type (fixed‑size unsigned integer or
 * arbitrary‑precision integer) and is valid C11/C++17.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static prime cache ------------------------------------------------------- */
static uint8_t *prime_cache = NULL;   /* 1 = prime, 0 = composite */
static idx_t prime_cap   = 0;          /* current capacity of prime_cache */

/* extend prime cache to cover all numbers up to max_need (inclusive) */
static void extend_prime_cache(idx_t max_need)
{
    if (prime_cap > max_need) return;        /* already sufficient */

    /* allocate larger array */
    size_t new_cap = max_need + 1;
    uint8_t *new_cache = (uint8_t *)calloc(new_cap, sizeof(uint8_t));
    if (!new_cache) return;                  /* out of memory: ignore */

    /* initially mark everything as prime (1) */
    memset(new_cache, 1, new_cap);

    /* 0 and 1 are not prime */
    if (new_cap > 0) new_cache[0] = 0;
    if (new_cap > 1) new_cache[1] = 0;

    /* sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= max_need; ++p) {
        if (new_cache[p]) {
            for (idx_t j = p * p; j <= max_need; j += p)
                new_cache[j] = 0;
        }
    }

    /* replace old cache */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cap   = new_cap;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* offset is 1 */

    /* maximum possible prefix sum after n steps */
    idx_t max_sum = 5 * n;

    /* ensure prime cache covers all sums we will test */
    extend_prime_cache(max_sum);

    /* allocate DP arrays */
    size_t arr_len = (size_t)max_sum + 1;
    seq_t *dp   = (seq_t *)calloc(arr_len, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(arr_len, sizeof(seq_t));
    if (!dp || !newdp) {                    /* memory failure: return 0 */
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;                       /* empty prefix sum */

    for (idx_t step = 1; step <= n; ++step) {
        idx_t max_prev = 5 * (step - 1);     /* reachable sums before this step */
        idx_t max_cur  = 5 * step;          /* after this step */
        /* iterate over all achievable previous sums */
        for (idx_t s = 0; s <= max_prev; ++s) {
            if (dp[s] == (seq_t)0) continue;   /* no sequences lead to this sum */
            /* try all possible next elements 1..5 */
            for (int a_val = 1; a_val <= 5; ++a_val) {
                idx_t new_sum = s + (idx_t)a_val;
                if (!prime_cache[new_sum]) {     /* allowed prefix sum */
                    newdp[new_sum] += dp[s];
                }
            }
        }
        /* prepare for next iteration: swap pointers and clear newdp */
        seq_t *tmp = dp;
        dp   = newdp;
        newdp = tmp;
        memset(newdp, 0, arr_len * sizeof(seq_t));
    }

    /* sum all counts in dp to obtain total number of valid sequences */
    seq_t total = (seq_t)0;
    for (idx_t s = 0; s <= max_sum; ++s) {
        total += dp[s];
    }

    /* free temporary arrays */
    free(dp);
    free(newdp);

    return total;
}
