/*
 * A352908  Numbers k such that A232324(k) is prime.
 * https://oeis.org/A352908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 2988, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_prime_small, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = k where the k-th triangular number T_k = k*(k+1)/2
 * modulo the sum of divisors σ(k) is a prime.
 * The function returns the n-th such k (offset 1).
 */
#include "../utils/sigma_of.h"
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state to remember progress across calls */
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t current_k = 1;      /* next k to test */

    /* Generate terms until we have at least n */
    while (seq_len < n) {
        seq_t T = (seq_t)current_k * ((seq_t)current_k + 1) / 2;
        seq_t sigma = sigma_of(current_k);
        seq_t rem = T % sigma;            /* triangular mod sigma */

        /* If the remainder is prime, store k */
        if (is_prime_small((uint64_t)rem)) {
            ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq_arr, sizeof(seq_t));
            seq_arr[seq_len++] = (seq_t)current_k;
        }
        ++current_k;
    }

    /* Return the requested term */
    return seq_arr[n - 1];
}
