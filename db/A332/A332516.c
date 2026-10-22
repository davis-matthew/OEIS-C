/*
 * A332516  Numbers k such that phi(k) == 10 (mod 12), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A332516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer k such that φ(k) ≡ 10 (mod 12)
 * Range:     u8 n <= 21, u16 n <= 2529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches integers in order, computes φ(k) by
 * trial division using a pre‑computed prime table, caches the results,
 * and stores the sequence for fast subsequent queries. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------------------------------------------------- */
/* Global state for the sequence and prime tables.     */
/* ---------------------------------------------------- */
static seq_t *terms = NULL;          /* cached sequence values   */
static size_t terms_len = 0;         /* number of elements cached */
static size_t terms_cap = 0;         /* current capacity          */
static uint64_t last_checked = 0;    /* last integer examined     */

static uint32_t *primes = NULL;      /* list of primes            */
static size_t prime_cnt = 0;         /* number of primes stored   */
static bool primes_ready = false;    /* flag: prime list built?  */

/* ---------------------------------------------------- */
/* Build a table of primes up to a fixed limit.        */
/* ---------------------------------------------------- */
static void init_primes(void)
{
    if (primes_ready)
        return;

    /* Limit large enough for all calls needed for n ≤ 10000. */
    const size_t LIMIT = 2000000;   /* ~2·10⁶ -> primes > 10⁶ for sqrt(x) */
    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));  /* false = prime candidate */

    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }
    }

    /* Count primes first. */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++prime_cnt;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!sieve[i])
            primes[idx++] = (uint32_t)i;
    }

    free(sieve);
    primes_ready = true;
}

/* ---------------------------------------------------- */
/* Resize the terms array when we need more space.     */
/* ---------------------------------------------------- */
static void grow_terms(size_t new_len)
{
    if (terms_cap >= new_len)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < new_len)
        new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* ---------------------------------------------------- */
/* Compute the Euler totient of a 64‑bit integer.      */
/* ---------------------------------------------------- */
static uint64_t phi_uint64(uint64_t n)
{
    init_primes();
    uint64_t result = n;
    uint64_t m = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0) {
            result = result / p * (p - 1);
            while (m % p == 0)
                m /= p;
        }
    }

    if (m > 1)           /* remaining prime factor > sqrt(n) */
        result = result / m * (m - 1);

    return result;
}

/* ---------------------------------------------------- */
/* Public entry point.                                 */
/* ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence offset is 1 */

    /* Ensure we have at least n terms computed. */
    while (terms_len < (size_t)n) {
        ++last_checked;
        if (phi_uint64(last_checked) % 12 == 10) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)last_checked;
        }
    }

    return terms[(size_t)n - 1];
}
