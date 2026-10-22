/*
 * A390946  Irregular triangle read by rows of continued fraction expansions
 *          of prime(i)/prime(j+1) for j >= 1, 1 <= i < j.
 * https://oeis.org/A390946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8858, u16 n <= 8858, u32 n <= 8858, u64 n <= 8858,
 *            u128 n <= 8858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A390946.
   The sequence consists of the continued‑fraction partial quotients
   (without the leading 0) of prime[i]/prime[j] for 0 ≤ i < j,
   arranged row‑wise by increasing j.  Each term is produced in
   the order they appear when the fractions are expanded.  The
   function maintains static state so that calls with any n are
   handled without recomputation. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static buffers for generated sequence and primes */
static seq_t *cache      = NULL; /* sequence terms */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static uint32_t *primes   = NULL; /* list of primes */
static size_t primes_len  = 0;
static size_t primes_cap  = 0;

/* indices of the next fraction to produce */
static size_t next_d = 1; /* denominator index (starting at 1) */
static size_t next_i = 0; /* numerator index (0 .. next_d-1) */

/* Add the next prime after the current list to primes[]. */
static void add_next_prime(void)
{
    uint32_t candidate = (primes_len == 0) ? 2U : primes[primes_len - 1] + 1U;
    for (;; ++candidate) {
        bool prime = true;
        for (size_t p = 0; p < primes_len; ++p) {
            uint32_t pr = primes[p];
            uint64_t pr2 = (uint64_t)pr * (uint64_t)pr;
            if (pr2 > candidate) break;
            if (candidate % pr == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
            return;
        }
    }
}

/* Ensure that primes[0 .. need] exist. */
static void ensure_prime_index(size_t need)
{
    while (primes_len <= need) add_next_prime();
}

/* Push a generated continued‑fraction term into cache. */
static void push_term(idx_t q)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)q;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but safe */

    /* If we already have enough terms, return immediately. */
    if (cache_len >= (size_t)n) return cache[n - 1];

    /* Initiate primes array if empty. */
    if (primes_len == 0) {
        primes_cap = 16;
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) abort();
        primes[0] = 2U;
        primes_len = 1;
    }

    /* Generate terms until we reach n. */
    while (cache_len < (size_t)n) {
        /* Make sure the denominator prime is available. */
        ensure_prime_index(next_d);

        uint64_t num = primes[next_i];
        uint64_t den = primes[next_d];

        /* Euclidean algorithm to produce CF terms. */
        while (num > 0) {
            uint64_t q  = den / num;
            uint64_t r  = den % num;
            push_term(q);
            den = num;
            num = r;
        }

        /* Advance to the next fraction. */
        ++next_i;
        if (next_i == next_d) {
            next_i = 0;
            ++next_d;
        }
    }

    return cache[n - 1];
}
