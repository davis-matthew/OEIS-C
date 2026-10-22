/*
 * A368732  Primes whose index is one, another prime number, or a semiprime.
 * https://oeis.org/A368732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime whose 1‑based index is either 1, prime, or
 *            semiprime (product of exactly two primes, with multiplicity)
 * Range:     u8 n <= 35, u16 n <= 2608, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* Determine whether x is a semiprime (exactly two prime factors,
   counting multiplicity).  x >= 2.  The function assumes that
   primes[] contains all primes up to at least sqrt(x). */
static bool is_semiprime(idx_t x, const uint64_t *primes, size_t primes_cnt)
{
    if (x < 4)               /* 4 = 2*2 is the smallest semiprime */
        return false;
    idx_t tmp = x;
    int count = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        while (tmp % p == 0) {
            ++count;
            tmp /= p;
            if (count > 2)
                return false;
        }
    }
    if (tmp > 1)              /* remaining prime factor */
        ++count;
    return count == 2;
}

/* The sequence.  The function keeps a static cache of the needed primes
   and the list of all primes up to a pre‑set limit.  It grows the cache
   until the n‑th term is available.  The limit 5 000 000 is ample for
   n up to about 10 000 and keeps the computation fast. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *seq = NULL;          /* cached terms */
    static size_t seq_len = 0;         /* number of terms stored */
    static size_t seq_cap = 0;         /* capacity of seq */

    static uint64_t *prime_list = NULL;  /* all primes up to SIEVE_LIMIT */
    static size_t prime_cnt = 0;
    static idx_t next_prime_idx = 1;     /* next prime index to examine (1‑based) */

    const idx_t SIEVE_LIMIT = 5000000;   /* enough for >10 000 needed terms */

    /* Initial construction of the prime list. */
    if (!initialized) {
        build_primes_upto(SIEVE_LIMIT, &prime_list, &prime_cnt);
        seq = NULL;
        seq_cap = 0;
        initialized = true;
    }

    /* Grow sequence until we have at least n terms. */
    while (seq_len < n) {
        /* Safety guard – in practice we should never run out of primes. */
        if (next_prime_idx > prime_cnt)
            break;

        bool ok = false;
        if (next_prime_idx == 1) {
            ok = true;                       /* index 1 is always accepted */
        } else if (is_prime(next_prime_idx)) {
            ok = true;                       /* index itself is prime */
        } else if (is_semiprime(next_prime_idx, prime_list, prime_cnt)) {
            ok = true;                       /* index is a semiprime */
        }

        if (ok) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)prime_list[next_prime_idx - 1];
        }

        ++next_prime_idx;
    }

    return seq[n - 1];
}
