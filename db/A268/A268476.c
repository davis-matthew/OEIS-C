/*
 * A268476  Balanced evil primes: primes with an even number of runs of 1's in
 *          their binary expansion.
 * https://oeis.org/A268476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th balanced evil prime
 * Range:     u8 n <= 28, u16 n <= 3239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   runs_of_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced evil primes: primes with an even number of runs of 1's in binary. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/runs_of_ones.h"

/* Static storage for the sequence. */
static seq_t *balanced = NULL;
static size_t balanced_cnt = 0;
static size_t balanced_cap = 0;
static uint64_t last_checked = 2;
static bool initialized = false;

/* Simple primality test for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return false;
    }
    return true;
}

/* Ensure that at least n balanced evil primes have been computed. */
static void ensure_balanced_list(idx_t n) {
    while (balanced_cnt < n) {
        if (is_prime_uint64(last_checked)) {
            int runs = runs_of_ones(last_checked);
            if ((runs & 1) == 0) {            /* even number of runs */
                if (balanced_cnt == balanced_cap) {
                    balanced_cap = balanced_cap ? balanced_cap << 1 : 8;
                    balanced = (seq_t *)realloc(balanced,
                                                balanced_cap * sizeof(seq_t));
                    if (!balanced) abort();
                }
                balanced[balanced_cnt++] = (seq_t)last_checked;
            }
        }
        ++last_checked;
    }
}

/* Public function for the OEIS benchmark. */
seq_t a(idx_t n) {
    if (n == 0) return 0;      /* out of range – not expected for valid inputs */
    ensure_balanced_list(n);
    return balanced[n - 1];
}
