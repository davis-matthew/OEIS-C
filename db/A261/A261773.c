/*
 * A261773  Number of full reptend primes p < n in base n.
 * https://oeis.org/A261773
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of primes p < n for which n is a primitive root
 *            modulo p (i.e., 1/p has maximal period p−1 in base n)
 * Range:     u8 n <= 728, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/build_primes.h"

/* helper: test if p is a full reptend prime in base n */
static bool is_full_reptend(idx_t p, idx_t n)
{
    /* p must be coprime to n */
    if (n % p == 0)
        return false;

    seq_t base = (seq_t)(n % p);
    seq_t mod  = (seq_t)p;

    idx_t tot = p - 1;
    idx_t m   = tot;

    /* factor tot and test primitive root condition */
    for (idx_t q = 2; q * q <= m; ++q) {
        if (m % q == 0) {
            seq_t res = mod_pow(base, (uint64_t)(tot / q), mod);
            if (res == (seq_t)1)
                return false;
            while (m % q == 0)
                m /= q;
        }
    }
    if (m > 1) {
        seq_t res = mod_pow(base, (uint64_t)(tot / m), mod);
        if (res == (seq_t)1)
            return false;
    }
    return true;
}

/* static cache of primes up to the largest n requested so far */
static idx_t *prime_arr   = NULL;
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* ensure we have primes up to limit (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    if (prime_arr)
        free(prime_arr);
    build_primes(limit, &prime_arr, &prime_cnt);
    prime_limit = limit;
}

/* public routine: sequence a(n) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* value below offset is irrelevant */

    idx_t needed = n - 1;
    ensure_primes(needed);

    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (p >= n)
            break;
        if (is_full_reptend(p, n))
            ++count;
    }
    return count;
}
