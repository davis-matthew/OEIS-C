/*
 * A332657  Alternate adding and multiplying prime numbers: a(2n) = a(2n-1) *
 *          prime(2n+1) and a(2n-1) = a(2n) + prime(2n-2) for n >= 1.
 * https://oeis.org/A332657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternate adding and multiplying prime numbers:
 * a(1) = prime(1)+prime(2).
 * For n >= 1:
 *   a(2n)   = a(2n-1) * prime(2n+1);
 *   a(2n-1) = a(2n-2) + prime(2n);   (for odd indices >1)
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Prime cache (1‑based indexing: prime_seq[0] = 2, etc.) */
static uint64_t *prime_seq = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static uint64_t prime_last_checked = 1;   /* last tested composite */

/* Allocate or grow the prime array. */
static void ensure_prime_cap(size_t need) {
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_seq, new_cap * sizeof(uint64_t));
    prime_seq = tmp;
    prime_cap = new_cap;
}

/* Generate the next prime and store it. */
static void generate_next_prime(void) {
    uint64_t cand = prime_last_checked + 1;
    for (;; ++cand) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_seq[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_seq[prime_cnt++] = cand;
            prime_last_checked = cand;
            return;
        }
    }
}

/* Return the k‑th prime (1‑based). */
static uint64_t get_prime_at(idx_t k) {
    if (k == 0) return 0;           /* safety */
    while (prime_cnt < (size_t)k) generate_next_prime();
    return prime_seq[k - 1];
}

/* Cache for computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;            /* number of computed terms */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need) {
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Multiplication with 128‑bit intermediate for 64‑bit builds. */
static seq_t mul128(seq_t a, uint64_t b) {
    unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)b;
    return (seq_t)prod;
}

/* Required function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset is 1, return 0 for safety */

    if ((idx_t)cache_len >= n) return seq_cache[n - 1];

    ensure_cache_cap((size_t)n);

    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)get_prime_at(1) + (seq_t)get_prime_at(2);
        } else if ((i & 1) == 0) {          /* even index */
            val = mul128(seq_cache[i - 2], get_prime_at(i + 1));
        } else {                            /* odd index */
            val = seq_cache[i - 2] + (seq_t)get_prime_at(i + 1);
        }
        seq_cache[i - 1] = val;
    }
    cache_len = n;
    return seq_cache[n - 1];
}
