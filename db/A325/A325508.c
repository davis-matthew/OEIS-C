/*
 * A325508  Product of primes indexed by the prime exponents of n!.
 * https://oeis.org/A325508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p ≤ n} prime( v_p(n!) ), where v_p(n!) is the
 *            exponent of p in n!
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented in pure C/C++ using only integer arithmetic as required. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Primes generation up to a fixed limit. This limit comfortably covers
   all indices needed for n ≤ 10000 (index ~ 10000, 10000th prime ≈ 104729). */
#define SIEVE_LIMIT 200000

/* Static storage for primes and state. */
static uint64_t primes[SIEVE_LIMIT];
static size_t primes_count = 0;
static bool primes_initialized = false;

/* Build primes up to SIEVE_LIMIT. */
static void build_primes(void)
{
    static uint8_t is_composite[SIEVE_LIMIT + 1];
    memset(is_composite, 0, sizeof(is_composite));

    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[(size_t)i]) {
            /* Add prime */
            primes[primes_count++] = i;
            if (i * i <= SIEVE_LIMIT) {
                for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[(size_t)j] = 1;
            }
        }
    }
}

/* --------------------------------------------------------------- */
/* The sequence function.                                            */
seq_t a(idx_t n)
{
    /* Ensure primes are available. */
    if (!primes_initialized) {
        build_primes();
        primes_initialized = true;
    }

    /* By definition a(0) = a(1) = 1. */
    if (n <= 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    uint64_t nn = (uint64_t)n;   /* promotion to 64-bit for safety */

    /* Iterate over all primes ≤ n. */
    for (size_t i = 0; i < primes_count && primes[i] <= nn; ++i) {
        uint64_t p = primes[i];
        uint64_t exponent = 0;
        uint64_t div = p;

        /* Compute exponent of prime p in n!. */
        while (div <= nn) {
            exponent += nn / div;
            /* Prevent overflow: if next multiplication would exceed nn, break. */
            if (div > nn / p) break;
            div *= p;
        }

        if (exponent > 0) {
            /* exponent is 1‑based index into the primes list. */
            /* Safety: exponent-1 must be < primes_count because exponent ≤ n (≈10000)
               and primes_count with SIEVE_LIMIT=200000 is >> 10000. */
            seq_t prime_at_index = (seq_t)primes[exponent - 1];
            result *= prime_at_index;
        }
    }

    return result;
}
