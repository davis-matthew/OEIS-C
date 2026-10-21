/*
 * A240960  Numbers m such that sigma(m) - phi(m) = tau(m)^omega(m), where
 *          sigma=A000203, phi=A000010, tau=A000005 and omega=A001221.
 * https://oeis.org/A240960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sigma(m) - phi(m) == tau(m)^omega(m)
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) returns the n‑th smallest positive integer m
   for which the above equality holds.  The calculation is done
   incrementally; a static cache stores already found terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Build‑supplied types */
extern uint64_t _smallest_prime_factor(uint64_t); /* unused but noted */

static const size_t MAX_LIMIT = 5000000; /* sufficient for n ≤ 10000 */

/* Sieve data structures */
static bool *is_prime = NULL;
static uint32_t *primes = NULL;
static size_t prime_count = 0;

/* Sequence cache */
static seq_t *seq_cache = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

/* Current integer being examined */
static idx_t current = 2;

/* One‑time sieve initialisation */
static void init_sieve(void)
{
    is_prime = (bool *)malloc(MAX_LIMIT + 1);
    if (!is_prime) abort();
    for (size_t i = 0; i <= MAX_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= MAX_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Allocate prime list (enough room for ~MAX_LIMIT/log(MAX_LIMIT)) */
    primes = (uint32_t *)malloc((MAX_LIMIT / 10 + 100) * sizeof(uint32_t));
    if (!primes) abort();
    for (size_t i = 2; i <= MAX_LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = (uint32_t)i;
}

/* Append a term to the static cache */
static void append_value(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 8;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache) abort();
    }
    seq_cache[seq_len++] = v;
}

/* Test whether m satisfies sigma(m) - phi(m) == tau(m)^omega(m) */
static bool satisfies(idx_t m)
{
    idx_t temp = m;
    seq_t sigma = 1;
    seq_t phi   = m;
    seq_t tau   = 1;
    size_t omega = 0;

    /* Trial division using primes up to sqrt(temp) */
    for (size_t i = 0; i < prime_count &&
            (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            size_t exp = 0;
            seq_t sum_div = 1;
            seq_t term = 1;
            seq_t P = p;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                term *= p;
                sum_div += term;
            }
            sigma *= sum_div;
            tau *= (seq_t)(exp + 1);
            ++omega;
            phi = phi / P * (P - 1);
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t P = (seq_t)temp;
        sigma *= (1 + P);
        tau   *= 2;
        ++omega;
        phi = phi / P * (P - 1);
    }

    /* Compute tau^omega */
    seq_t rhs = 1;
    for (size_t i = 0; i < omega; ++i) rhs *= tau;

    return (sigma - phi) == rhs;
}

/* Return the n-th term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    /* Initialise sieve and cache once */
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        bool is_p = false;
        if (current <= MAX_LIMIT) {
            is_p = is_prime[(size_t)current];
        }
        if (is_p || satisfies(current)) {
            append_value((seq_t)current);
        }
        ++current;
    }
    return seq_cache[(size_t)n - 1];
}
