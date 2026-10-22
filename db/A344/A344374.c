/*
 * A344374  a(n) is the number of squarefree numbers appearing in the 1st
 *          column of an n X n square array whose elements are the numbers
 *          from 1..n^2, listed in increasing order by rows.
 * https://oeis.org/A344374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(1 + n*(k-1))^2 (Count squarefree numbers
 *            in the first column of the n×n array)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Static state for prime generation used in square‑free tests */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit  = 0;

/* Simple trial‑division test for square‑freeness, using prime_list */
static inline bool is_sqfree_u64(uint64_t v) {
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > v) break;
        if (v % p == 0) {
            v /= p;
            if (v % p == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; n>=1 in practice */

    /* Ensure prime_list covers all primes up to n */
    if (n > prime_limit) {
        free(prime_list);
        build_primes_upto(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t x = 1 + (uint64_t)n * (k - 1);
        if (is_sqfree_u64(x)) {
            count += (seq_t)1;
        }
    }
    return count;
}
