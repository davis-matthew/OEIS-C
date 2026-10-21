/*
 * A261581  Primes such that z(p) is not divisible by 4 where z(n) is
 *          A214028(n), the smallest k such that n divides A000129(k), the
 *          k-th Pell number.
 * https://oeis.org/A261581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime such that z(p) is not divisible by 4, where
 *            z(p) is the smallest k with p | Pell(k)
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mul_matrix, next_prime, rank_of_apparition, zero_at_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mul_matrix.h"
#include "../utils/rank_of_apparition.h"
#include "../utils/zero_at_index.h"

static seq_t *good = NULL;
static size_t good_cap = 0;
static size_t good_cnt = 0;
static uint64_t last_checked = 1;  // last prime considered

/* Ensure the array 'good' has enough capacity. */
static void grow_good(void) {
    if (good_cap == 0) {
        good_cap = 16;
        good = (seq_t*)malloc(sizeof(seq_t) * good_cap);
    } else if (good_cnt >= good_cap) {
        good_cap <<= 1;
        good = (seq_t*)realloc(good, sizeof(seq_t) * good_cap);
    }
}

/* Build the list of good primes until it contains at least 'need' entries. */
static void build_good_until(size_t need) {
    if (good_cnt >= need) return;
    while (good_cnt < need) {
        last_checked = next_prime(last_checked);
        idx_t z = rank_of_apparition(last_checked);
        if (z % 4 != 0) {
            grow_good();
            good[good_cnt++] = (seq_t)last_checked;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                // undefined for offset 1
    build_good_until((size_t)n);
    return good[n - 1];
}
