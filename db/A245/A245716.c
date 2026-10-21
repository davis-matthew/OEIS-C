/*
 * A245716  Least number k > 0 such that n + k! and n - k! are both prime, or
 *          0 if no such k exists.
 * https://oeis.org/A245716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4764, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes least k>0 such that n +/- k! are both prime, or 0 if none exists.
   For a(n)>0 we have k! < n, so only k up to 20 need be checked. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t fact[21];
static bool fact_inited = false;

static void init_factorials(void) {
    if (fact_inited) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i <= 20; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_inited = true;
}

seq_t a(idx_t n) {
    init_factorials();
    if (n < 2) return (seq_t)0;        /* trivial small n */

    seq_t sn = (seq_t)n;
    for (int k = 1; k <= 20; ++k) {
        seq_t fk = fact[k];
        if (fk >= sn) break;          /* k! is not < n any more */
        seq_t below = sn - fk;
        seq_t above = sn + fk;
        if (is_prime_big(below) && is_prime_big(above)) {
            return (seq_t)k;
        }
    }
    return (seq_t)0;
}
