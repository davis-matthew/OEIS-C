/*
 * A376331  a(n) is the smallest product of n consecutive primes p_1..p_k,
 *          where only p_1 < m^(1/n).
 * https://oeis.org/A376331
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implementation of A376331.
 * a(n) = product of n consecutive primes p_i … p_{i+n-1}
 *   such that p_i  < (product)^(1/n) < p_{i+1}.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

static uint64_t *primes = NULL;
static size_t      prime_cnt = 0;
static size_t      prime_limit = 0;

/* Extend the prime table until we have at least <needed> primes. */
static void ensure_primes(size_t needed)
{
    if (prime_cnt >= needed)
        return;                     /* already sufficient */

    /* Grow the limit geometrically until we get enough primes. */
    size_t lim = prime_limit > 0 ? prime_limit : 104729; /* ≈ 10^5th prime */
    while (prime_cnt < needed) {
        size_t new_lim = lim * 2;
        uint64_t *new_pr = NULL;
        size_t new_cnt = 0;

        build_primes_upto(new_lim, &new_pr, &new_cnt);
        free(primes);            /* free old table (malloced by build) */
        primes     = new_pr;
        prime_cnt  = new_cnt;
        prime_limit = new_lim;
        lim = new_lim;
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* sequence offset starts at n = 2 */

    /* initialise primes once */
    if (!primes) {
        prime_limit = 2000000;    /* gives ~148 933 primes – plenty for n≤10 000 */
        build_primes_upto(prime_limit, &primes, &prime_cnt);
    }

    /* we need at least n+1 primes for the first window */
    ensure_primes(n + 1);

    /* product of the first n primes */
    seq_t prod = 1;
    for (idx_t j = 0; j < n; ++j)
        prod *= (seq_t)primes[j];

    /* slide the window until we find the required segment */
    for (idx_t i = 0;; ++i) {
        /* guarantee we can refer to primes[i+1] and primes[i+n] */
        ensure_primes(i + n + 1);

        seq_t low  = seq_pow((seq_t)primes[i],       n); /* p_i  ^ n */
        seq_t high = seq_pow((seq_t)primes[i + 1],   n); /* p_{i+1} ^ n */

        if (prod > low && prod < high)
            return prod;        /* found the minimal product */

        /* slide window: remove primes[i], add primes[i+n] */
        prod = (prod / (seq_t)primes[i]) * (seq_t)primes[i + n];
    }
}
