/*
 * A350958  Positive numbers k such that 2k cannot be written as 3^i (i >= 0)
 *          plus a prime.
 * https://oeis.org/A350958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th positive integer k such that 2k cannot be
 * written as 3^i + p for any prime p and integer i ≥ 0. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *prime_sieve = NULL;
static uint64_t sieve_limit = 0;

static void ensure_sieve(uint64_t limit)
{
    if (sieve_limit >= limit) return;
    free(prime_sieve);
    prime_sieve = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    if (!prime_sieve) exit(1);
    /* mark 2..limit as prime initially */
    for (uint64_t i = 2; i <= limit; ++i) prime_sieve[i] = 1;
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (prime_sieve[p]) {
            for (uint64_t j = p * p; j <= limit; j += p) prime_sieve[j] = 0;
        }
    }
    sieve_limit = limit;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n <= sieve_limit) return prime_sieve[n];
    /* fallback: trial division */
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (p <= sieve_limit) {
            if (!prime_sieve[p]) continue;
        }
        if (n % p == 0) return false;
    }
    return true;
}

/* cache for the sequence */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
/* next candidate k to test */
static uint64_t next_k_to_test = 1;

static void add_to_cache(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap << 1;
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but return 0 */

    while (seq_len < (size_t)n) {
        uint64_t k = next_k_to_test;
        uint64_t two_k = k * 2;
        ensure_sieve(two_k);

        bool bad = false;
        uint64_t pow3 = 1;
        while (pow3 <= two_k) {
            uint64_t val = two_k - pow3;
            if (is_prime_uint64(val)) {
                bad = true;
                break;
            }
            /* avoid overflow */
            if (pow3 > two_k / 3) break;
            pow3 *= 3;
        }
        if (!bad) {
            add_to_cache((seq_t)k);
        }
        ++next_k_to_test;
    }

    return seq_cache[(size_t)n - 1];
}
