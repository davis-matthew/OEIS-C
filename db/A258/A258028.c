/*
 * A258028  Numbers k such that D(prime(k), 3) > 0, where D( * , 3) = 3rd
 *          difference.
 * https://oeis.org/A258028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258028: Numbers k such that D(prime(k),3) < 0,
   where D(p,3) = P(k+3) - 3*P(k+2) + 3*P(k+1) - P(k). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple primality test using the primes already computed. */
static bool _is_prime_uint64(uint64_t n, uint64_t *primes, size_t primes_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        unsigned __int128 sq = (unsigned __int128)p * p;
        if (sq > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Ensure capacity for a dynamic array. */
static void _ensure_capacity(size_t *cap, void **ptr, size_t item_size)
{
    if (*cap == 0) {
        *cap = 128;
        *ptr = malloc(*cap * item_size);
    } else {
        *cap <<= 1;
        *ptr = realloc(*ptr, *cap * item_size);
    }
}

/* Return the n-th element (1–based) of sequence A258028. */
seq_t a(idx_t n)
{
    /* Static state: primes, negative-D k-values. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;

    static seq_t *neg_k = NULL;   /* terms where D < 0 */
    static size_t neg_cnt = 0;
    static size_t neg_cap = 0;

    static uint64_t candidate = 2;   /* next number to test for primality */

    while (neg_cnt < n) {          /* need more terms */
        /* Find next prime */
        while (!_is_prime_uint64(candidate, primes, primes_cnt))
            ++candidate;

        /* Store the prime */
        if (primes_cnt == primes_cap)
            _ensure_capacity(&primes_cap, (void **)&primes, sizeof(uint64_t));
        primes[primes_cnt++] = candidate;
        ++candidate;               /* next candidate (skip even numbers) */

        /* Compute the third difference for k = primes_cnt-3 if we have >=4 primes */
        if (primes_cnt >= 4) {
            uint64_t p0 = primes[primes_cnt - 4];   /* P(k)   */
            uint64_t p1 = primes[primes_cnt - 3];   /* P(k+1) */
            uint64_t p2 = primes[primes_cnt - 2];   /* P(k+2) */
            uint64_t p3 = primes[primes_cnt - 1];   /* P(k+3) */

            signed __int128 d =
                (signed __int128)p3
              - (signed __int128)3 * p2
              + (signed __int128)3 * p1
              - (signed __int128)p0;

            if (d < 0) {                 /* D < 0 → include k = primes_cnt-3 */
                if (neg_cnt == neg_cap)
                    _ensure_capacity(&neg_cap, (void **)&neg_k, sizeof(seq_t));
                neg_k[neg_cnt++] = (seq_t)(primes_cnt - 3);
            }
        }
    }

    return neg_k[n - 1];
}
