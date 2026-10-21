/*
 * A280076  Numbers n such that Sum_{d|n} tau(d) = Product_{d|n} tau(d).
 * https://oeis.org/A280076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1; otherwise a(n) = p_{n-1}^2, where p_k denotes
 *            the k‑th prime (2,3,5,7,…)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 6543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Prime cache */
static uint64_t *g_primes = NULL;
static size_t    g_primes_cnt = 0;
static idx_t     g_limit = 100;

/* Ensure that at least `need` primes are available. */
static void ensure_primes(idx_t need)
{
    if (g_primes_cnt >= (size_t)need)
        return;

    while (g_primes_cnt < (size_t)need)
    {
        /* Double the sieve limit until enough primes are generated. */
        if (g_limit < 2)
            g_limit = 2;
        g_limit *= 2;

        /* Rebuild the prime list. */
        if (g_primes)
            free(g_primes);
        size_t new_cnt = 0;
        build_primes_upto(g_limit, &g_primes, &new_cnt);
        g_primes_cnt = new_cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;        /* 1 is included as a special case */

    idx_t k = n - 1;             /* k-th prime needed (k ≥ 1) */
    ensure_primes(k);            /* guarantee at least k primes are stored */

    uint64_t prime = g_primes[(size_t)(k - 1)];
    seq_t res = (seq_t)prime * (seq_t)prime;
    return res;
}
