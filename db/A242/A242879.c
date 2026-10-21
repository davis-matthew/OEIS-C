/*
 * A242879  Least positive integer k < n such that k*p == 1 (mod prime(k)) for
 *          some prime p < prime(k) and (n-k)*q == 1 (mod prime(n-k)) for some
 *          prime q < prime(n-k), or 0 if such a number k does not exist.
 * https://oeis.org/A242879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k < n (k>0) such that * k * p ≡ 1 (mod
 *            prime(k)) for some prime p < prime(k) * and (n-k) * q ≡ 1 (mod
 *            prime(n-k)) for some prime q < prime(n-k). * If no such k
 *            exists, a(n) = 0. * This implementation precomputes prime
 *            numbers up to the n‑th prime * using a simple sieve and tests...
 * Range:     u8 n <= 59, u16 n <= 7976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"
#include "../utils/is_prime_small.h"

static idx_t *prime_list = NULL;   /* 0‑based array of primes: prime_list[0] = 2 */
static size_t prime_count = 0;     /* number of primes stored */

/* Ensure that prime_list contains at least n primes (indices 0 .. n-1). */
static void ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;                     /* already have enough */

    idx_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = n * 20 + 10;        /* generous upper bound for the n‑th prime */

    size_t cnt = 0;
    idx_t *list = NULL;

    /* Sieve until we have at least n primes. */
    while (1) {
        sieve_primes(limit, &list, &cnt);
        if (cnt >= n)
            break;
        /* Not enough primes: increase bound and retry. */
        limit *= 2;
        free(list);
    }

    free(prime_list);
    prime_list = list;
    prime_count = cnt;
}

/* Modular inverse of a modulo mod (mod is prime), returns 0 if not invertible. */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        uint64_t q = r / newr;
        int64_t tmp = newt;
        newt = t - (int64_t)q * newt;
        t = tmp;
        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }
    if (r > 1)
        return 0;          /* not invertible, but should not happen here */
    if (t < 0)
        t += mod;
    return (uint64_t)t;
}

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;

    ensure_primes(n);          /* need primes up to n (indices 0 .. n-1) */

    for (idx_t k = 1; k < n; ++k) {
        idx_t pk = prime_list[k - 1];          /* prime(k) */
        uint64_t invk = modinv_uint64((uint64_t)k, (uint64_t)pk);
        if (invk < 2 || !is_prime_small(invk))
            continue;

        idx_t nk = n - k;
        idx_t pnk = prime_list[nk - 1];        /* prime(n-k) */
        uint64_t invnk = modinv_uint64((uint64_t)nk, (uint64_t)pnk);
        if (invnk < 2 || !is_prime_small(invnk))
            continue;

        /* Both conditions satisfied: return this least k. */
        return (seq_t)k;
    }
    return 0;    /* no suitable k found */
}
