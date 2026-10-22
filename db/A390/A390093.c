/*
 * A390093  Numbers k > 1 such that the least prime index of k and the
 *          greatest prime index of k are both odd.
 * https://oeis.org/A390093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the set of numbers k > 1 for which
   the least prime index of k and the greatest prime index of k
   are both odd.  The sequence is defined as the intersection of
   A340932 and A244991, so we generate it by a simple sieve and
   checking the prime‑factor indices of each integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static seq_t *sequence = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

static uint32_t *spf = NULL;          /* smallest prime factor */
static uint32_t *prime_idx = NULL;    /* 1‑based index of a prime, 0 if composite */

#define SIEVE_LIMIT 200000   /* enough to generate >10 000 terms */

/* Allocate arrays with casting to satisfy C++. */
static void init_sieve(void)
{
    uint32_t i, p;
    size_t count = 0;
    size_t primes_cap = SIEVE_LIMIT;
    uint32_t *primes = (uint32_t *)calloc(primes_cap, sizeof(uint32_t));

    spf = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    prime_idx = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));

    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            primes[count++] = i;
            prime_idx[i] = (uint32_t)count;  /* 1‑based index of prime i */
        }
        for (size_t j = 0; j < count; ++j) {
            p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > SIEVE_LIMIT)
                break;
            spf[i * p] = p;
        }
    }
    free(primes);

    /* Build the sequence up to SIEVE_LIMIT */
    for (uint32_t k = 2; k <= SIEVE_LIMIT; ++k) {
        uint32_t x = k;
        uint32_t min_idx = UINT32_MAX;
        uint32_t max_idx = 0;

        while (x > 1) {
            uint32_t q = spf[x];
            uint32_t idx = prime_idx[q];
            if (idx < min_idx) min_idx = idx;
            if (idx > max_idx) max_idx = idx;
            do { x /= q; } while (x % q == 0);
        }

        if ( (min_idx & 1U) && (max_idx & 1U) ) {   /* both odd */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                sequence = (seq_t *)realloc(sequence,
                                              seq_cap * sizeof(seq_t));
            }
            sequence[seq_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sieve();
        initialized = true;
    }
    /* n is 1‑based; check bounds to avoid UB */
    if ((size_t)n == 0 || (size_t)n > seq_len)
        return (seq_t)0;               /* undefined for out‑of‑range */
    return sequence[(size_t)n - 1];
}
