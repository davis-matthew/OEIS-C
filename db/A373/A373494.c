/*
 * A373494  Numbers k for which A059975(k) and A003415(k) are both multiples
 *          of 3, where A059975 is fully additive with a(p) = p-1, and A003415
 *          is the arithmetic derivative.
 * https://oeis.org/A373494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer k such that A059975(k) and
 *            A003415(k) are both multiples of 3. Conditions on the prime
 *            factorisation of k: let e3 = exponent of prime 3, S1 = sum of
 *            exponents of primes ≡ 1 (mod 3) (mod 3), S2 = sum of exponents
 *            of primes ≡ 2 (mod 3) (mod 3). Then k is in the seq...
 * Range:     u8 n <= 28, u16 n <= 6478, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static bool primes_initialised = false;

/* ------------------------------------------------------------- */
/*  Simple sieve to collect primes up to LIMIT (1 000 000).       */
/* ------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_initialised) return;
    const uint64_t LIMIT = 1000000;      // enough for all expected values
    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) return;          // unlikely, but safe

    size_t capacity = 80000;            // enough for primes <= LIMIT
    prime_array = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!prime_array) { free(is_composite); return; }

    for (uint64_t p = 2; p <= LIMIT; ++p) {
        if (!is_composite[p]) {
            if (prime_count == capacity) {      // grow if needed
                capacity <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_array,
                                                    capacity * sizeof(uint64_t));
                if (!tmp) break;                 // allocation fail
                prime_array = tmp;
            }
            prime_array[prime_count++] = p;
            if (p * p <= LIMIT) {
                for (uint64_t q = p * p; q <= LIMIT; q += p)
                    is_composite[q] = true;
            }
        }
    }
    free(is_composite);
    primes_initialised = true;
}

/* ------------------------------------------------------------- */
/*  Check whether candidate num satisfies the OEIS conditions.   */
/* ------------------------------------------------------------- */
static bool good_candidate(idx_t num)
{
    uint64_t n = (uint64_t)num;             // real integer to factor
    uint8_t S1_mod = 0;                     // sum of exponents of p≡1 (mod3)
    uint8_t S2_mod = 0;                     // sum of exponents of p≡2 (mod3)
    int    e3_exp = 0;                      // exponent of prime 3

    init_primes();

    size_t i;
    for (i = 0; i < prime_count && prime_array[i] * prime_array[i] <= n; ++i) {
        uint64_t p = prime_array[i];
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            if (p == 3) {
                e3_exp += exp;
            } else if (p % 3 == 1) {
                S1_mod = (S1_mod + exp) % 3;
            } else {          // p % 3 == 2
                S2_mod = (S2_mod + exp) % 3;
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (n > 1) {
        if (n == 3ULL) {
            e3_exp += 1;
        } else if (n % 3 == 1) {
            S1_mod = (S1_mod + 1) % 3;
        } else {                 // n % 3 == 2
            S2_mod = (S2_mod + 1) % 3;
        }
    }

    /* Apply conditions */
    if (e3_exp == 1)
        return false;
    if (e3_exp == 0)
        return S1_mod == 0 && S2_mod == 0;
    /* e3_exp >= 2 */
    return S2_mod == (uint8_t)(e3_exp % 3);
}

/* ------------------------------------------------------------- */
/*  Cache of accepted terms to allow out‑of‑order queries.        */
/* ------------------------------------------------------------- */
static seq_t *term_cache = NULL;
static size_t cache_size = 0;
static size_t cache_capacity = 0;
static idx_t next_candidate = 1;   // next integer to test

/* Ensure that term_cache contains at least n terms (0‑based). */
static void extend_cache(idx_t n)
{
    init_primes();

    if (cache_capacity == 0) {
        cache_capacity = 128;
        term_cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!term_cache) return;   // allocation failure - shouldn't happen
    }

    while (cache_size < n) {
        if (good_candidate(next_candidate)) {
            if (cache_size == cache_capacity) {
                cache_capacity <<= 1;
                seq_t *tmp = (seq_t *)realloc(term_cache, cache_capacity * sizeof(seq_t));
                if (!tmp) break;   // allocation fail
                term_cache = tmp;
            }
            term_cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------- */
/*  Public sequence function.                                    */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset‑1 sequences */
    extend_cache(n);
    return term_cache[n - 1];     /* a(1) is first term in cache[0] */
}
