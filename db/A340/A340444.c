/*
 * A340444  a(n) is the least prime of the form p*q + p*r + q*r where p is the
 *          n-th prime and q and r are primes < p, or 0 if there are none.
 * https://oeis.org/A340444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least prime of the form * p*q + p*r + q*r where p =
 *            prime(n) and q,r are distinct primes < p, * or 0 if no such
 *            prime exists. The function searches primes * in increasing order
 *            and stops when a candidate cannot be smaller * than the current
 *            best. It uses the provided utilities for * genera...
 * Range:     u8 n <= 8, u16 n <= 517, u32 n <= 1415, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* cached list of primes up to the current limit */
static uint64_t *cached_primes = NULL;
static size_t    cached_nprimes = 0;
static size_t    cached_limit   = 0;

/* build primes up to at least 'limit' (in primes) */
static void ensure_primes_upto(idx_t limit)
{
    if ((size_t)limit <= cached_limit) return;

    /* free any previously allocated array */
    if (cached_primes) free(cached_primes);

    build_primes_upto(limit, &cached_primes, &cached_nprimes);
    cached_limit = (size_t)limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* nth prime */
    seq_t p = (seq_t)get_prime(n);

    /* have primes up to p */
    ensure_primes_upto((idx_t)p);

    /* number of primes less than p is n-1 */
    const size_t lim = (size_t)(n - 1);

    seq_t best = 0;
    bool   found = false;

    for (size_t i = 0; i + 1 < lim; ++i) {
        seq_t q = (seq_t)cached_primes[i];

        for (size_t j = i + 1; j < lim; ++j) {
            seq_t r = (seq_t)cached_primes[j];

            seq_t val = p * q + p * r + q * r;

            if (found && val >= best) break;

            if (!is_prime_big(val)) continue;

            best = val;
            found = true;
            break;          /* further r will only increase val */
        }

        if (found) {
            /* lower bound for next i */
            if (i + 2 < lim) {
                seq_t qn = (seq_t)cached_primes[i + 1];
                seq_t rn = (seq_t)cached_primes[i + 2];
                seq_t lower = p * qn + p * rn + qn * rn;
                if (lower >= best) break;
            }
        }
    }

    return found ? best : 0;
}
