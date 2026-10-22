/*
 * A369661  Numbers k whose arithmetic derivative k' is of the form 4m+2, and
 *          k' has an even number of prime factors.
 * https://oeis.org/A369661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 2744, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A369661: numbers k such that the arithmetic derivative k' ≡ 2 (mod 4)
 * and k' has an even number of prime factors (counted with multiplicity). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Prime generation (used only for trial division). */
static uint32_t *primes = NULL;
static size_t num_primes = 0;
static seq_t *seq_cache = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;
static uint64_t next_k = 1;

/* Build a list of primes up to LIMIT (sufficient for the required range). */
static void build_primes(void)
{
    if (num_primes) return;
    const uint32_t LIMIT = 200000;                 /* covers k up to ~4·10¹⁰ */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    memset(is_prime, 1, (LIMIT + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++num_primes;
    primes = (uint32_t*)malloc(num_primes * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;
    free(is_prime);
}

/* Ensure the cache has at least “needed” elements. */
static void ensure_seq_cache(size_t needed)
{
    if (needed <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

/* Count prime factors of d (with multiplicity).  Return true if the count
 * is even.  The function assumes d fits in seq_t and that the prime list
 * is long enough to factor it. */
static bool has_even_prime_factors(seq_t d)
{
    int cnt = 0;
    for (size_t i = 0; i < num_primes; ++i) {
        uint32_t p = primes[i];
        if ((seq_t)p * (seq_t)p > d) break;
        while (d % (seq_t)p == 0) {
            ++cnt;
            d /= (seq_t)p;
        }
    }
    if (d != (seq_t)1) ++cnt;   /* remaining prime factor */
    return (cnt & 1) == 0;
}

/* Test whether k satisfies the defining property of the sequence. */
static bool is_good(uint64_t k)
{
    if (k < 2) return false;
    seq_t kseq = (seq_t)k;
    seq_t derivative = (seq_t)0;
    uint64_t rem = k;
    for (size_t i = 0; i < num_primes; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > rem) break;
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            derivative += kseq / (seq_t)p * (seq_t)e;
        }
    }
    if (rem > 1) {                      /* remaining prime factor */
        derivative += kseq / (seq_t)rem; /* e = 1 */
    }

    /* Check congruence modulo 4. */
    if ((derivative & 3) != 2) return false;

    /* Check that the number of prime factors of derivative is even. */
    return has_even_prime_factors(derivative);
}

/* Return the n-th term of the sequence.  The function is memoised; calls
 * may come in any order. */
seq_t a(idx_t n)
{
    static bool primes_ready = false;
    if (!primes_ready) { build_primes(); primes_ready = true; }

    /* Extend the cached sequence until we have at least n terms. */
    while (seq_cache_len < (size_t)n) {
        while (1) {
            if (is_good(next_k)) {
                ensure_seq_cache(seq_cache_len + 1);
                seq_cache[seq_cache_len++] = (seq_t)next_k;
                ++next_k;                /* advance to the next candidate */
                break;
            }
            ++next_k;
        }
    }
    return seq_cache[(size_t)n - 1];
}
