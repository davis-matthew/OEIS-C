/*
 * A341676  The unique superior prime divisor of each number that has one.
 * https://oeis.org/A341676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the unique superior prime divisor of the n-th positive
 *            integer m that has one. A divisor d|m is superior if d ≥ m/d.
 *            For every m, the single prime divisor ≥ sqrt(m), if it exists,
 *            is the unique superior prime divisor. The function returns these
 *            primes in order of the integers that possess...
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"  /* for any auxiliary utilities if needed */

/* We need a fast integer square‑root.  Use the helper. */
#include "../utils/gcd.h"  /* same header contains _floor_isqrt_u64 */

#define SIEVE_LIMIT 500000U  /* limits for sieve – enough for n up to many thousands */

static uint64_t *spf = NULL;          /* smallest prime factor array */
static idx_t   spf_limit = 0;
static void init_spf(void) {
    if (spf != NULL) return;
    spf_limit = SIEVE_LIMIT;
    spf = (uint64_t*)malloc((size_t)(spf_limit + 1) * sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);
    memset(spf, 0, (size_t)(spf_limit + 1) * sizeof(uint64_t));
    for (uint64_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;                     /* i is prime */
            if (i * i <= spf_limit) {
                for (uint64_t j = i * i; j <= spf_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf[1] = 1;
}

/* Dynamic array to store the sequence values */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static idx_t processed_up_to = 1;  /* last m for which we have examined */

/* Append a value to the dynamic array, resizing if needed */
static void append_seq(seq_t v) {
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *p = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!p) exit(EXIT_FAILURE);
        seq = p;
        seq_cap = new_cap;
    }
    seq[seq_len++] = v;
}

/* The main Oracle function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1, but guard */
    /* Ensure sieve is ready */
    init_spf();

    /* Generate sequence items until we have at least n */
    while (seq_len < n) {
        if (processed_up_to >= spf_limit) {
            /* For safety: if we exceed the sieve we stop; in practice
               SIEVE_LIMIT is more than sufficient for requested n. */
            break;
        }
        ++processed_up_to;
        uint64_t m = (uint64_t)processed_up_to;
        /* Find largest prime factor using spf */
        uint64_t temp = m;
        uint64_t largest = 1;
        while (temp > 1) {
            uint64_t p = spf[temp];
            if (p > largest) largest = p;
            while (temp % p == 0) temp /= p;
        }
        /* Check if largest prime >= sqrt(m) */
        if (largest * largest >= m) {
            append_seq((seq_t)largest);
        }
    }

    if (n <= seq_len) {
        return seq[n - 1];  /* 0‑based index */
    } else {
        /* If we ran out of precomputed data, the caller requested
           an index beyond our limit; return 0 to indicate error. */
        return 0;
    }
}
