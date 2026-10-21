/*
 * A289494  Number of primes in the interval [3n, 4n].
 * https://oeis.org/A289494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes in the interval [3n , 4n] = π(4n) –
 *            π(3n‑1) (π(x) = number of primes ≤ x)
 * Range:     u8 n <= 2310, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  n is 1‑based (offset = 1).  The function is memoised
   internally but works correctly for any order of calls.  It
   returns a value of type seq_t, which must be an unsigned
   integer type (or an arbitrary‑precision class that can be
   constructed from an integer).  All intermediate counts are
   held in uint64_t, which is sufficient for the required
   range (n ≤ 10 000 ⇒ 4n ≤ 40 000). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --- local static cache ------------------------------------------ */

static bool    *prime_flag = NULL; /* true = composite, false = prime */
static uint64_t *pi_arr     = NULL; /* prefix prime counts */
static size_t   cur_limit   = 0;     /* maximum number currently sieved */

/* Rebuild sieve and pi array up to (inclusive) 'limit'. */
static void rebuild(size_t limit)
{
    /* Allocate flag array, zero‑initialised */
    bool *new_flag = (bool*)calloc(limit + 1, sizeof(bool));
    if (!new_flag) abort();

    /* Mark 0 and 1 as composite */
    if (limit >= 0) new_flag[0] = true;
    if (limit >= 1) new_flag[1] = true;

    /* Classic Eratosthenes sieve */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!new_flag[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                new_flag[m] = true;
        }
    }

    /* Allocate and fill prefix prime count array */
    uint64_t *new_pi = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_pi) abort();
    new_pi[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        new_pi[i] = new_pi[i - 1] + (!new_flag[i] ? (uint64_t)1 : (uint64_t)0);

    /* Replace old tables (free first) */
    free(prime_flag);
    free(pi_arr);
    prime_flag = new_flag;
    pi_arr     = new_pi;
    cur_limit  = limit;
}

/* Compute a(n) – number of primes in [3n,4n] */
seq_t a(idx_t n)
{
    if (n == 0)  /* safety – offsets start at 1 */
        return (seq_t)0;

    idx_t start = 3 * n;
    idx_t end   = 4 * n;

    /* Ensure sieve covers up to 'end' */
    if (!prime_flag || (size_t)end > cur_limit)
        rebuild((size_t)end);

    uint64_t pi_end            = pi_arr[(size_t)end];
    uint64_t pi_start_minus_one = pi_arr[(size_t)(start - 1)];

    return (seq_t)(pi_end - pi_start_minus_one);
}
