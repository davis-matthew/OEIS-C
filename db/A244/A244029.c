/*
 * A244029  Primes p such that the quadratic form x^2+p*y^2 does not represent
 *          a prime strictly between p and 2p.
 * https://oeis.org/A244029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the quadratic form x^2 + p*y^2 does not
   represent a prime strictly between p and 2p.  The function a(n) gives
   the n‑th such prime (offset 1).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Floor integer square root. */
static idx_t isqrt_idx(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Sieve of Eratosthenes data. */
static uint8_t *sieve_is_comp = NULL;
static idx_t sieve_limit = 0;          /* inclusive maximum index in sieve */
static uint64_t *prime_list = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* Extend the sieve up to at least limit. */
static void ensure_sieve(idx_t limit) {
    if (sieve_limit >= limit) return;
    idx_t new_limit = limit;
    size_t new_cap = (size_t)new_limit + 1;   /* +1 for 0 index */

    uint8_t *new_is_comp = (uint8_t *)realloc(sieve_is_comp, new_cap);
    if (!new_is_comp) exit(1);
    sieve_is_comp = new_is_comp;
    for (size_t i = sieve_limit + 1; i < new_cap; ++i) sieve_is_comp[i] = 0;

    if (new_limit >= 0) sieve_is_comp[0] = 1;
    if (new_limit >= 1) sieve_is_comp[1] = 1;

    idx_t sqrt_new = isqrt_idx(new_limit);
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = prime_list[pi];
        if (p > sqrt_new) break;
        idx_t start = (sieve_limit / p + 1) * p;
        if ((idx_t)p * p > start) start = (idx_t)p * p;
        for (idx_t j = start; j <= new_limit; j += p)
            sieve_is_comp[j] = 1;
    }

    for (idx_t i = sieve_limit + 1; i <= new_limit; ++i) {
        if (!sieve_is_comp[i]) {
            if (primes_cnt == primes_cap) {
                size_t nc = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, nc * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                primes_cap = nc;
            }
            prime_list[primes_cnt++] = (uint64_t)i;
        }
    }
    sieve_limit = new_limit;
}

/* Test primality of an idx_t value using the sieve. */
static inline bool isPrimeVal(idx_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;
    ensure_sieve(n);
    return !sieve_is_comp[n];
}

/* Storage for qualifying primes. */
static seq_t *qual_list = NULL;
static size_t qual_cnt = 0;
static size_t qual_cap = 0;
static idx_t next_prime_candidate = 3;   /* start searching from 3 */

/* Helper to append a qualifying prime. */
static void append_qual(idx_t p) {
    if (qual_cnt == qual_cap) {
        size_t nc = qual_cap ? qual_cap * 2 : 16;
        if (qual_cap == 0) {
            qual_list = seq_alloc(nc);
        } else {
            qual_list = seq_realloc(qual_list, qual_cap, nc);
        }
        qual_cap = nc;
    }
    qual_list[qual_cnt++] = (seq_t)p;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 0, safe fallback */

    while (qual_cnt < n) {
        /* Find next prime. */
        idx_t p = next_prime_candidate;
        while (!isPrimeVal(p)) p += 2;
        next_prime_candidate = p + 2;

        /* Check whether any prime of the form p + x^2 lies strictly
           between p and 2p. */
        bool has_rep = false;
        idx_t max_x = isqrt_idx(p - 1);
        for (idx_t x = 1; x <= max_x; ++x) {
            idx_t q = p + x * x;
            if (q <= 2 * p && isPrimeVal(q)) {
                has_rep = true;
                break;
            }
        }
        if (!has_rep) append_qual(p);
    }

    return qual_list[n - 1];
}
