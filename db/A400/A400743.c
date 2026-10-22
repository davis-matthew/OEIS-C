/*
 * A400743  Primes p such that the multiplicative order of 16 modulo p is
 *          even.
 * https://oeis.org/A400743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = nth prime p such that the multiplicative order of 16
 *            modulo p is even (i.e., the multiplicative order of 2 modulo p
 *            is divisible by 8). The function searches primes up to a fixed
 *            limit and records those that satisfy the condition. The list is
 *            generated once and reused for subsequent calls
 * Range:     u8 n <= 4, u16 n <= 547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static seq_t   *seq_vals = NULL;   /* array of found primes */
static size_t   seq_len  = 0;      /* number of stored primes      */
static size_t   seq_cap  = 0;      /* allocated capacity           */
static bool     initialized = false;

static void
init_seq(void)
{
    if (initialized) return;

    /* ---------- sieve of Eratosthenes up to LIMIT ---------- */
    const idx_t LIMIT = 10000000;                /* 10 million */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                       /* allocation failure, abort */
    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    idx_t sqrt_limit = 0;
    while ((sqrt_limit + 1) * (sqrt_limit + 1) <= LIMIT) ++sqrt_limit;

    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* ---------- collect all primes into an array ---------- */
    size_t primes_count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++primes_count;

    uint32_t *primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return;
    }
    idx_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) primes[idx++] = (uint32_t)i;
    }

    /* ---------- allocate space for the sequence ---------- */
    seq_cap = 20000;  /* initial capacity */
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;

    /* ---------- test each prime for the property ---------- */
    for (size_t pi = 0; pi < primes_count; ++pi) {
        uint32_t p = primes[pi];
        if (p == 2) continue;
        if ((p & 15) != 1) continue;   /* p % 16 == 1 */

        /* Compute the multiplicative order of 2 modulo p */
        uint64_t m   = (uint64_t)p - 1;
        uint64_t ord = m;
        uint64_t temp = m;

        /* factor temp using the list of primes */
        size_t fi = 0;
        while (fi < primes_count && (uint64_t)primes[fi] * primes[fi] <= temp) {
            uint32_t q = primes[fi];
            if (temp % q == 0) {
                while (temp % q == 0) temp /= q;
                while (ord % q == 0) {
                    seq_t cand = mod_pow((seq_t)2, ord / q, (seq_t)p);
                    if (cand == (seq_t)1) ord /= q;
                    else break;
                }
            }
            ++fi;
        }
        if (temp > 1) {
            uint32_t q = (uint32_t)temp;
            while (ord % q == 0) {
                seq_t cand = mod_pow((seq_t)2, ord / q, (seq_t)p);
                if (cand == (seq_t)1) ord /= q;
                else break;
            }
        }

        if ((ord & 7ULL) == 0) {   /* ord divisible by 8 */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)p;
        }
    }

    /* ---------- cleanup temporary data ---------- */
    free(is_prime);
    free(primes);

    initialized = true;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;      /* offset 1: a(1) is first term */
    if (!initialized) init_seq();
    if (n - 1 < seq_len) return seq_vals[n - 1];
    /* If requested index exceeds precomputed list, return 0. */
    return 0;
}
