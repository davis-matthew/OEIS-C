/*
 * A256145  Primitive prime factors of the cyclotomic polynomial sequence
 *          Phi(8,k) in the order in which they occur.
 * https://oeis.org/A256145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256145: primitive prime factors of k^4 + 1,
   listed in the order of their first appearance as k increases. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper: floor of square root for a seq_t (works for 64‑bit and / or big integers). */
static seq_t floor_sqrt_seq(seq_t x) {
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Simple sieve generating all primes <= limit.
   The primes array is allocated by the function; caller must free it. */
static void generate_primes(uint64_t limit, uint64_t **primes_out, size_t *cnt_out) {
    bool *is_composite = (bool*)calloc(limit + 1, sizeof(bool));
    if (!is_composite) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t estimated = limit / ( (int)(__builtin_log2(limit)) + 1 ) + 10;
    uint64_t *pr = (uint64_t*)malloc(estimated * sizeof(uint64_t));
    size_t pc = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (pc == estimated) {
                estimated *= 2;
                pr = (uint64_t*)realloc(pr, estimated * sizeof(uint64_t));
            }
            pr[pc++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);
    *primes_out = pr;
    *cnt_out = pc;
}

/* Append a new term to the sequence cache. */
static void cache_append(seq_t val, seq_t **cache, size_t *len, size_t *cap) {
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        *cache = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    (*cache)[(*len)++] = val;
}

/* Main function: returns the nth term of the sequence. */
seq_t a(idx_t n) {
    /* Static state for cache and primality bookkeeping */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t *prime_list = NULL;
    static uint64_t primes_limit = 0;
    static size_t primes_cnt = 0;
    static bool *seen = NULL;
    static size_t seen_cap = 2000000;   /* initial size for seen flags */

    /* Allocate seen array once */
    if (!seen) {
        seen = (bool*)calloc(seen_cap, sizeof(bool));
    }

    /* Generate terms until we have at least n terms */
    static uint64_t last_k = 0;   /* last processed k */
    while (cache_len < (size_t)n) {
        last_k++;
        seq_t k = (seq_t)last_k;
        seq_t k2 = k * k;
        seq_t k4 = k2 * k2;
        seq_t x = k4 + 1;          /* k^4 + 1 */

        seq_t m = x;
        seq_t sqrtm = floor_sqrt_seq(x);

        /* Ensure we have all primes up to sqrt(m) */
        if ((uint64_t)sqrtm > primes_limit) {
            free(prime_list);
            generate_primes((uint64_t)sqrtm, &prime_list, &primes_cnt);
            primes_limit = (uint64_t)sqrtm;
        }

        /* Factor m using the primes list */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = prime_list[i];
            seq_t p_seq = (seq_t)p;
            if (p_seq * p_seq > m) break;
            if (m % p_seq == 0) {
                if (p < seen_cap) {
                    if (!seen[p]) {
                        seen[p] = true;
                        cache_append((seq_t)p, &cache, &cache_len, &cache_cap);
                    }
                } else {
                    /* enlarge seen array if needed */
                    size_t new_cap = p + 1;
                    seen = (bool*)realloc(seen, new_cap * sizeof(bool));
                    for (size_t j = seen_cap; j < new_cap; ++j) seen[j] = false;
                    seen_cap = new_cap;
                    seen[p] = true;
                    cache_append((seq_t)p, &cache, &cache_len, &cache_cap);
                }
                while (m % p_seq == 0) m /= p_seq;
            }
        }
        /* If remaining factor is >1, it is a prime */
        if (m > 1) {
            uint64_t mp = (uint64_t)m;  /* m fits in 64‑bit for the ranges considered */
            if (mp < seen_cap) {
                if (!seen[mp]) {
                    seen[mp] = true;
                    cache_append(m, &cache, &cache_len, &cache_cap);
                }
            } else {
                size_t new_cap = mp + 1;
                seen = (bool*)realloc(seen, new_cap * sizeof(bool));
                for (size_t j = seen_cap; j < new_cap; ++j) seen[j] = false;
                seen_cap = new_cap;
                seen[mp] = true;
                cache_append(m, &cache, &cache_len, &cache_cap);
            }
        }
    }

    return cache[n - 1];
}
