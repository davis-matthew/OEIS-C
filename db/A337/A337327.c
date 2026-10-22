/*
 * A337327  Maximum value of the cyclic self-convolution of the first n terms
 *          of the characteristic function of primes.
 * https://oeis.org/A337327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = maximum over k=0..n-1 of * sum_{i=0}^{n-1} χ_prime(i+1) *
 *            χ_prime(n-1-i-k+1) * where χ_prime(n) is 1 if n is prime and 0
 *            otherwise
 * Range:     u8 n <= 2711, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache of primes up to the largest n used so far. */
static uint8_t *prime_flag = NULL;      /* prime_flag[i] == 1 if i is prime, 0 otherwise (size = limit+1) */
static size_t prime_cap  = 0;           /* current capacity of prime_flag */

/* Ensure the prime_flag array contains primality information up to limit. */
static void ensure_primes(size_t limit) {
    if (limit < 2) return;
    if (limit <= prime_cap) return;

    uint8_t *new_flag = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!new_flag) exit(EXIT_FAILURE);

    /* initialize all to true (1) */
    for (size_t i = 0; i <= limit; ++i) new_flag[i] = 1;
    new_flag[0] = 0;
    new_flag[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (size_t j = i * i; j <= limit; j += i) new_flag[j] = 0;
        }
    }
    free(prime_flag);
    prime_flag = new_flag;
    prime_cap  = limit;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* Ensure primality data up to n. */
    ensure_primes((size_t)n);

    /* Build a list of indices (0‑based) of primes up to n. */
    size_t primecnt = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (prime_flag[i]) ++primecnt;
    }

    size_t *prime_idx = (size_t*)malloc(primecnt * sizeof(size_t));
    if (!prime_idx) exit(EXIT_FAILURE);
    size_t kpos = 0;
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (prime_flag[i]) prime_idx[kpos++] = i - 1;   /* store 0‑based index */
    }

    /* Allocate a temporary count array for each rotation. */
    static uint32_t *cnt = NULL;
    static size_t cnt_cap = 0;
    if ((size_t)n > cnt_cap) {
        free(cnt);
        cnt = (uint32_t*)malloc((size_t)n * sizeof(uint32_t));
        if (!cnt) exit(EXIT_FAILURE);
        cnt_cap = (size_t)n;
    }
    for (size_t i = 0; i < (size_t)n; ++i) cnt[i] = 0;

    /* Count pairs of primes, updating rotation counts. */
    for (size_t a = 0; a < primecnt; ++a) {
        size_t i = prime_idx[a];
        for (size_t b = 0; b < primecnt; ++b) {
            size_t j = prime_idx[b];
            /* Compute rotation index k = (n-1-i-j) mod n, always 0..n-1 */
            int t = (int)((size_t)n - 1 - i - j);   /* may be negative */
            if (t < 0) t += (int)n;
            cnt[(size_t)t]++;                       /* cast back to size_t for indexing */
        }
    }

    /* Find the maximum count. */
    uint32_t max_val = 0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        if (cnt[i] > max_val) max_val = cnt[i];
    }

    free(prime_idx);
    return (seq_t)max_val;
}
