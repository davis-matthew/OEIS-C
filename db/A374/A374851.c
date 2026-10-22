/*
 * A374851  Obverse convolution A000040**A000040; see Comments.
 * https://oeis.org/A374851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{n}(p_i + p_{n+1-i}) where p_i is the i‑th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_buf = NULL;
static size_t      prime_cnt = 0;

/* Generate first enough primes once */
static void init_primes(void) {
    if (prime_buf) return;                       /* already initialized */
    const size_t LIMIT = 200000U;                /* > 10000th prime (104729) */
    bool *comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!comp) exit(EXIT_FAILURE);

    /* Rough estimate of prime count up to LIMIT; a safe over‑estimate */
    const size_t EST = LIMIT / 3;
    prime_buf = (uint64_t*)malloc(EST * sizeof(uint64_t));
    if (!prime_buf) exit(EXIT_FAILURE);

    prime_cnt = 0;
    for (size_t num = 2; num <= LIMIT; ++num) {
        if (!comp[num]) {
            prime_buf[prime_cnt++] = (uint64_t)num;
            if (num <= LIMIT / num) {            /* avoid overflow of num*num */
                for (size_t mult = num * num; mult <= LIMIT; mult += num) {
                    comp[mult] = true;
                }
            }
        }
    }
    free(comp);
}

/* Main sequence function – external linkage */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;          /* undefined for n < 1 */
    init_primes();
    seq_t result = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        size_t idx1 = (size_t)(i - 1);            /* 0‑based index for p_i   */
        size_t idx2 = (size_t)(n - i);            /* 0‑based index for p_{n+1-i} */
        seq_t add = (seq_t)prime_buf[idx1] + (seq_t)prime_buf[idx2];
        result *= add;
    }
    return result;
}
