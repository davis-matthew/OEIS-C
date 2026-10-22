/*
 * A355531  Minimal augmented difference between adjacent reversed prime
 *          indices of n; a(1) = 0.
 * https://oeis.org/A355531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   minimal augmented difference between adjacent reversed prime
 *            indices of n. a(n) = minimal value of { q_k, q_i - q_{i+1} + 1
 *            (for i < k) } where q is the list of prime indices of n
 *            (reversed, i.e. decreasing)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_pi.h"

#define MAX_PRIME 100000  /* sieve up to this value for small primes */

static bool sieve[MAX_PRIME + 1];
static uint32_t primes[MAX_PRIME/10];   /* enough room for primes up to MAX_PRIME */
static uint16_t prime_index_map[MAX_PRIME + 1];
static size_t prime_count = 0;
static bool primes_initialized = false;

/* Initialise the small‑prime sieve and index table. */
static void init_primes(void) {
    if (primes_initialized) return;
    for (size_t i = 0; i <= MAX_PRIME; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i) sieve[j] = false;
        }
    }
    size_t idx = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            primes[idx]          = (uint32_t)i;
            prime_index_map[i]   = (uint16_t)(idx + 1);   /* index starts at 1 */
            ++idx;
        }
    }
    prime_count      = idx;
    primes_initialized = true;
}

/* Return the index of prime p.  For small primes use the precomputed table,
   otherwise use the supplied compute_pi routine. */
static inline seq_t prime_index(uint64_t p) {
    if (p <= MAX_PRIME && prime_index_map[(size_t)p])
        return (seq_t)prime_index_map[(size_t)p];
    return (seq_t)compute_pi((idx_t)p - 1) + 1;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n) {
    init_primes();

    if (n == 1) return 0;

    idx_t temp = n;
    seq_t indices[64];     /* sufficient for any n <= 2^63-1 */
    size_t count = 0;

    /* Factor using the precomputed small primes. */
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        while (temp % p == 0) {
            indices[count++] = prime_index(p);
            temp /= p;
        }
    }

    /* If a prime factor remains. */
    if (temp > 1) {
        indices[count++] = prime_index((uint64_t)temp);
    }

    if (count == 0) return 0;   /* should only happen for n==1 */

    seq_t min_val = indices[0];          /* smallest index (last in reversed list) */
    for (size_t i = 0; i + 1 < count; ++i) {
        seq_t diff = (seq_t)(indices[i + 1] - indices[i] + 1);
        if (diff < min_val) min_val = diff;
    }
    return min_val;
}
