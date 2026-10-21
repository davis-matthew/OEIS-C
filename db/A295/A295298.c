/*
 * A295298  Numbers n for which sum of the divisors (A000203) and the binary
 *          weight of n (A000120) have the same parity.
 * https://oeis.org/A295298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose sum of divisors * (σ(n))
 *            and binary weight of n (popcount) have the same parity
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Precomputed primes up to 200000 ----------------------------------- */
#define PRIME_LIMIT 200000
static uint32_t prime_list[PRIME_LIMIT];
static size_t prime_count = 0;
static void build_primes(void)
{
    static bool ready = false;
    if (ready) return;
    static unsigned char sieve[PRIME_LIMIT + 1];

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) prime_list[prime_count++] = (uint32_t)i;
    }
    ready = true;
}

/* --- Helpers ------------------------------------------------------------------ */
static bool sigma_parity_odd(uint64_t n)
{
    /* Remove all factors of 2 --- they do not affect parity for σ. */
    while ((n & 1ULL) == 0) n >>= 1;
    if (n == 1) return true; /* only 2-powers -> σ odd */

    /* Check odd prime factors */
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            unsigned e = 0;
            do { n /= p; ++e; } while (n % p == 0);
            if (e & 1) return false; /* odd exponent -> σ even */
        }
    }
    /* Remaining n > 1 is an odd prime with exponent 1 (odd). */
    if (n > 1) return false;
    return true; /* no odd prime with odd exponent */
}

static bool popcount_parity(uint64_t x)
{
    bool parity = false;
    while (x) {
        parity ^= (x & 1ULL);
        x >>= 1;
    }
    return parity;
}

/* --- Main sequence function --------------------------------------------------- */
seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t last_candidate = 0;

    if (n == 0) return 0; /* undefined; safety */

    build_primes();

    while (seq_len < n) {
        ++last_candidate;
        uint64_t cand = last_candidate;
        bool sigma_odd = sigma_parity_odd(cand);
        bool pop_odd   = popcount_parity(cand);
        if (sigma_odd == pop_odd) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *new_arr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!new_arr) return (seq_t)0; /* allocation failed */
                seq = new_arr;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)cand;
        }
    }
    return seq[n - 1];
}
