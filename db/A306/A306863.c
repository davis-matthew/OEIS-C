/*
 * A306863  a(n) is the number of primes between the n-th and (n+1)-st odd
 *          composite numbers.
 * https://oeis.org/A306863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes strictly between the n‑th and (n+1)‑st
 *            odd composite numbers
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned char *is_prime = NULL;   /* sieve array: 1 = prime, 0 = composite */
static size_t prime_limit = 0;          /* maximum index in is_prime */
static idx_t *odd_comp = NULL;          /* odd composite numbers */
static size_t comp_cap = 0;             /* capacity of odd_comp */
static idx_t comp_cnt = 0;              /* number of composites stored */
static idx_t last_checked_odd = 7;      /* last odd number examined (end of 7) */

/* Build a sieve of Eratosthenes up to 'limit' (inclusive). */
static void build_sieve(size_t limit) {
    free(is_prime);
    is_prime = (unsigned char*)malloc(limit + 1);
    if (!is_prime) exit(1); /* abort on allocation failure */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }
    prime_limit = limit;
}

/* Ensure the sieve covers numbers up to at least 'min_limit'. */
static void ensure_sieve(size_t min_limit) {
    if (min_limit <= prime_limit) return;
    /* grow a bit to reduce reseve frequency */
    if (min_limit < prime_limit * 2) min_limit = prime_limit * 2;
    if (min_limit < 32) min_limit = 32;
    build_sieve(min_limit);
}

/* Generate odd composites until we have at least 'need' entries (1‑based). */
static void ensure_composites(idx_t need) {
    if (need <= (idx_t)comp_cnt) return;
    while (comp_cnt < need) {
        last_checked_odd += 2;          /* next odd number */
        idx_t cand = last_checked_odd;
        if ((size_t)cand > prime_limit) {
            /* make sure sieve covers up to at least double the current candidate */
            ensure_sieve((size_t)(cand * 2));
        }
        if (!is_prime[(size_t)cand]) {   /* composite (odd candidate > 7) */
            if (comp_cnt == comp_cap) {
                comp_cap = comp_cap ? comp_cap * 2 : 16;
                odd_comp = (idx_t*)realloc(odd_comp, comp_cap * sizeof(idx_t));
                if (!odd_comp) exit(1);
            }
            odd_comp[comp_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but return 0 */
    /* Need n+1 composites to access nth and (n+1)th entries */
    ensure_composites(n + 1);
    idx_t lower = odd_comp[n - 1];
    idx_t upper = odd_comp[n];
    seq_t count = 0;
    for (idx_t p = lower + 1; p < upper; ++p) {
        if ((p & 1u) == 0) continue;                /* skip evens */
        if (is_prime[(size_t)p]) ++count;
    }
    return count;
}
