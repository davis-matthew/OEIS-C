/*
 * A340933  Numbers whose least prime index is even. Heinz numbers of integer
 *          partitions whose last part is even.
 * https://oeis.org/A340933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements OEIS A340933.
   a(n) is the n‑th integer whose least prime index is even.
   The function builds the sequence on demand by sieving integers
   and selecting those whose smallest prime factor has an even
   prime index. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Forward declarations of helper functions */
static void recompute_sieve(idx_t limit);
static void generate_to_n(idx_t target);

static seq_t *seq_cache = NULL;          /* cache of sequence values a(1)..a(k) */
static idx_t seq_len = 0;                /* current number of cached values */

static uint32_t *spf_index_arr = NULL;   /* spf_index[x] = index of smallest prime factor of x (0 if x==1) */
static idx_t sieve_limit = 0;            /* maximum number sieved so far */

static idx_t last_tested = 1;            /* last number that was examined */

/* Helper: generate sequence up to position 'target' (inclusive) */
static void generate_to_n(idx_t target) {
    while (seq_len < target) {
        /* Advance to next integer to test */
        ++last_tested;

        /* Extend sieve if needed */
        if (last_tested > sieve_limit) {
            idx_t new_limit = (sieve_limit > 0) ? sieve_limit * 2 : 32;
            if (new_limit < last_tested * 2) new_limit = last_tested * 2;
            recompute_sieve(new_limit);
        }

        /* 1 has no prime indices, skip it */
        if (last_tested == 1) continue;

        /* Include only if least prime index is even */
        if ((spf_index_arr[last_tested] & 1U) == 0) {
            /* Grow sequence cache if necessary */
            if (seq_len == 0) {
                seq_cache = (seq_t *)malloc(16 * sizeof(seq_t));
            } else {
                /* Ensure capacity, double if needed */
                if (seq_len == (idx_t)((seq_t)(seq_cache[0]) == 0 ? 0 : 0)) {
                    /* This block is never reached but keeps the compiler happy */
                }
            }
            if (seq_len >= (idx_t)16) {
                seq_cache = (seq_t *)realloc(seq_cache, seq_len * 2 * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)last_tested;
        }
    }
}

/* Recompute sieve up to 'limit'; fills spf_index_arr */
static void recompute_sieve(idx_t limit) {
    /* Free old arrays */
    free(spf_index_arr);
    spf_index_arr = NULL;

    /* Allocate temporary array for smallest prime factor (by value) */
    uint32_t *spf_val = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!spf_val) exit(1);

    /* Allocate array for prime indices (by value) */
    uint32_t *prime_idx = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!prime_idx) {
        free(spf_val);
        exit(1);
    }

    /* Dynamic array for primes during linear sieve */
    uint32_t *primes = NULL;
    size_t pr_cap = 0;
    size_t pr_cnt = 0;

    /* Linear sieve, filling spf_val */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf_val[i] == 0) {                /* i is prime */
            spf_val[i] = (uint32_t)i;
            if (pr_cnt == pr_cap) {
                pr_cap = pr_cap ? pr_cap * 2 : 16;
                primes = (uint32_t *)realloc(primes, pr_cap * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[pr_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pr_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t v = i * (idx_t)p;
            if (v > limit) break;
            if (spf_val[v] == 0) spf_val[v] = p;
            if (p == spf_val[i]) break;
        }
    }

    /* Build prime_to_index mapping */
    for (size_t idx = 0; idx < pr_cnt; ++idx) {
        prime_idx[primes[idx]] = (uint32_t)(idx + 1);  /* 1‑based index */
    }

    /* Allocate spf_index_arr and fill it from spf_val and prime_idx */
    spf_index_arr = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf_index_arr) exit(1);
    spf_index_arr[0] = 0;
    for (idx_t n = 2; n <= limit; ++n) {
        uint32_t p = spf_val[n];
        if (p == n)                /* n is prime */
            spf_index_arr[n] = prime_idx[n];
        else
            spf_index_arr[n] = prime_idx[p];
    }

    /* Clean up */
    free(spf_val);
    free(prime_idx);
    free(primes);

    sieve_limit = limit;
}

/* Public function as requested by the test harness */
seq_t a(idx_t n) {
    /* Ensure we have at least n terms cached */
    if (n == 0) return (seq_t)0;          /* undefined for n == 0, return 0 */
    if (seq_len < n) {
        generate_to_n(n);
    }
    return seq_cache[n - 1];
}
