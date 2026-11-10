/*
 * A349806  a(n) = A099610(n) - n^2.
 * https://oeis.org/A349806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A099610(n) - n^2, where A099610(n) is the smallest odd
 *            number > n^2 that is a product of two distinct primes; computed
 *            by a forward search
 * Range:     u8 n <= 15, u16 n <= 303, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 129 terms through n = 975 (2026-11-10)
 * Helpers:   build_primes_upto, is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"

static bool A349806_ready = false;
static uint64_t *A349806_primes = NULL;
static size_t A349806_count = 0;

static void A349806_init(void) {
    if (A349806_ready) return;
    build_primes_upto((idx_t)200000, &A349806_primes, &A349806_count);
    A349806_ready = true;
}

/* odd m is a product of two distinct odd primes */
static bool A349806_semi(seq_t m) {
    for (size_t i = 0; i < A349806_count; ++i) {
        uint64_t p = A349806_primes[i];
        if (p < 3) continue;
        seq_t sp = (seq_t)p;
        if (sp * sp > m) break;
        if (m % sp == 0) {
            seq_t q = m / sp;
            if (q != sp && is_prime_seq(q)) return true;
            return false;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    A349806_init();
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t m = sq;
    if ((m & 1) == 0) m += 1; else m += 2;
    for (;;) {
        if (A349806_semi(m)) return m - sq;
        m += 2;
    }
}
