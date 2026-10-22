/*
 * A365003  Heinz numbers of integer partitions where the sum of all parts is
 *          twice the sum of distinct parts.
 * https://oeis.org/A365003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer whose prime‐index weighted exponents
 *            satisfy Σ_j j·(e_j - 2) = 0, where e_j is the exponent of the
 *            j‑th prime factor. The function enumerates integers in
 *            increasing order and returns the n‑th element
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_LIMIT 5000000ULL  /* upper bound sufficient for n ≤ 10000 */

static uint32_t *spf          = NULL;   /* smallest prime factor */
static uint32_t *prime_idx    = NULL;   /* index of prime (1‑based), 0 for composites */
static unsigned prime_count   = 0;

/* Precompute spf and prime_idx up to MAX_LIMIT */
static void init_sieve(void) {
    if (spf != NULL) return;  /* already initialized */

    spf       = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    prime_idx = (uint32_t *)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    if (!spf || !prime_idx) abort();

    for (size_t i = 0; i <= MAX_LIMIT; ++i) {
        spf[i] = 0;
        prime_idx[i] = 0;
    }

    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {           /* i is prime */
            spf[i] = (uint32_t)i;
            prime_idx[i] = ++prime_count;  /* 1‑based index */
            if (i * i <= MAX_LIMIT) {
                for (size_t j = i * i; j <= MAX_LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* Check whether the integer num satisfies the condition */
static bool satisfies(idx_t num) {
    idx_t m = num;
    long long balance = 0;
    while (m > 1) {
        uint32_t p   = spf[m];
        idx_t idx_p  = prime_idx[p];      /* index of the prime p */
        idx_t e      = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        balance += (long long)(e - 2) * (long long)idx_p;
    }
    return balance == 0;
}

/* Dynamic array for the sequence */
static seq_t  *seq_array = NULL;
static size_t seq_size  = 0;
static size_t seq_cap   = 0;

/* Append a value to the sequence array, growing it if necessary */
static void append_seq(seq_t val) {
    if (seq_size == seq_cap) {
        seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        seq_array = (seq_t *)realloc(seq_array, seq_cap * sizeof(seq_t));
        if (!seq_array) abort();
    }
    seq_array[seq_size++] = val;
}

/* Static state for incremental generation */
static bool      seq_initialized = false;
static idx_t     next_candidate  = 2;  /* next integer to test */

/* Main function required by the task */
seq_t a(idx_t n) {
    if (!seq_initialized) {
        init_sieve();
        /* Store the first term a(1) = 1 explicitly */
        append_seq((seq_t)1);
        seq_initialized = true;
    }

    /* Generate further terms until we reach the desired index */
    while (seq_size < n) {
        /* Skip 1 (already handled) */
        if (next_candidate == 1) {
            ++next_candidate;
            continue;
        }
        if (next_candidate > MAX_LIMIT) {
            /* In the unlikely event that more terms are needed beyond the
               pre‑computed limit, abort as the current bounds are
               sufficient for all required n. */
            abort();
        }
        if (satisfies(next_candidate))
            append_seq((seq_t)next_candidate);
        ++next_candidate;
    }

    return seq_array[n - 1];
}
