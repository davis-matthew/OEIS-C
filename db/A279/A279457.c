/*
 * A279457  Numbers k such that number of distinct primes dividing k is odd
 *          and number of prime divisors (counted with multiplicity) of k is
 *          odd.
 * https://oeis.org/A279457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279457: numbers k such that both ω(k) (distinct prime
   factors) and Ω(k) (total prime factors counted with multiplicity)
   are odd.  No closed‑form formula is known, so the terms are generated
   by an incremental search and memoised. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Storage for the sequence. */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;
/* Prime table used for factorisation. */
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
/* Next integer to examine for primality (used by the sieve). */
static uint64_t prime_candidate = 2;

/* Ensure capacity for the sequence array. */
static void seq_ensure_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        if (newcap < needed) newcap = needed;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* Ensure capacity for the primes array. */
static void primes_ensure_capacity(size_t needed)
{
    if (primes_cap < needed) {
        size_t newcap = primes_cap ? primes_cap * 2 : 16;
        if (newcap < needed) newcap = needed;
        primes = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
        primes_cap = newcap;
    }
}

/* Incremental prime sieve up to and including n. */
static void generate_primes_up_to(uint64_t n)
{
    while (prime_candidate <= n) {
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > prime_candidate / p) break;          /* p*p > candidate  */
            if (prime_candidate % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            primes_ensure_capacity(primes_len + 1);
            primes[primes_len++] = prime_candidate;
        }
        ++prime_candidate;
    }
}

/* Return true iff k satisfies the required parity conditions. */
static bool qualifies(uint64_t k)
{
    uint64_t temp = k;
    int omega = 0, bigomega = 0;

    for (size_t i = 0; i < primes_len && primes[i] <= temp / primes[i]; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            ++omega;
            while (temp % p == 0) {
                temp /= p;
                ++bigomega;
            }
        }
    }
    if (temp > 1) {            /* remaining prime factor */
        ++omega;
        ++bigomega;
    }
    return (omega & 1) && (bigomega & 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safety beyond offset */

    /* Find terms until we have at least n of them. */
    static uint64_t current_candidate = 2;     /* next integer to test */
    while (seq_len < (size_t)n) {
        /* Ensure the prime table is sufficient for factorisation. */
        generate_primes_up_to(current_candidate);
        if (qualifies(current_candidate)) {
            seq_ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }
    return seq[n - 1];
}
