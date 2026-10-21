/*
 * A283022  Numbers k such that the number of primes of the form k - x is not
 *          equal to the number of primes of the form k + y where x, y are
 *          divisors of k.
 * https://oeis.org/A283022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the number of primes of the
 *            * form k - d (with d a divisor of k) is NOT equal to the number
 *            of primes of * the form k + d (with d a divisor of k)
 * Range:     u8 n <= 44, u16 n <= 7729, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(seq_t)          */
#include "../utils/append_candidate.h"/* append_candidate(seq_t**,size_t*,size_t*,seq_t) */

static seq_t *cached_seq = NULL;   /* dynamic array of sequence terms          */
static size_t cached_len = 0;      /* number of terms stored                  */
static size_t cached_cap = 0;      /* capacity of the array                   */
static uint64_t next_candidate = 1;/* next k to test (starting at 1)          */

/* Simple integer square root: floor(sqrt(x)) */
static idx_t isqrt_u64(uint64_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* Count primes among numbers k - d and k + d for all divisors d of k. */
static void count_prime_pairs(uint64_t k, int *cnt_minus, int *cnt_plus) {
    *cnt_minus = *cnt_plus = 0;
    idx_t r = isqrt_u64(k);
    for (idx_t d = 1; d <= r; ++d) {
        if (k % d == 0) {
            /* divisor d */
            uint64_t dm1 = k - d;
            if (dm1 >= 2 && is_prime_small((seq_t)dm1)) (*cnt_minus)++;
            uint64_t dp1 = k + d;
            if (is_prime_small((seq_t)dp1)) (*cnt_plus)++;

            /* paired divisor k/d if different */
            if (d != k / d) {
                uint64_t d2 = k / d;
                uint64_t dm2 = k - d2;
                if (dm2 >= 2 && is_prime_small((seq_t)dm2)) (*cnt_minus)++;
                uint64_t dp2 = k + d2;
                if (is_prime_small((seq_t)dp2)) (*cnt_plus)++;
            }
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset is 1, but guard anyway */

    /* Generate terms until we have at least n terms */
    while (cached_len < n) {
        uint64_t k = next_candidate++;

        /* Count primes for k-d and k+d over all divisors of k */
        int cnt_minus = 0, cnt_plus = 0;
        count_prime_pairs(k, &cnt_minus, &cnt_plus);
        if (cnt_minus != cnt_plus) {
            /* Store this k as a term of the sequence */
            append_candidate(&cached_seq, &cached_len, &cached_cap, (seq_t)k);
        }
    }

    return cached_seq[n - 1];
}
