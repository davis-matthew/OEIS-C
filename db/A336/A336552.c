/*
 * A336552  Numbers k such that A003557(k)-1 either divides A326143(k) [=
 *          A001065(k) - A007947(k)], or both are zero. Numbers k such that
 *          gcd(A336551(k), A326143(k)) is equal to A336551(k).
 * https://oeis.org/A336552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that (A003557(k)-1) | A326143(k) (A326143(k)
 *            = A001065(k)-A007947(k)), or both are 0; found by search with
 *            cached terms
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9040 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A336552_test(uint64_t k) {
    uint64_t t = k, rad = 1, sig = 1;
    for (uint64_t p = 2; p <= t / p; p++) {
        if (t % p == 0) {
            uint64_t pp = 1, sum = 1;
            while (t % p == 0) {
                t /= p;
                pp *= p;
                sum += pp;
            }
            sig *= sum;
            rad *= p;
        }
    }
    if (t > 1) {
        sig *= (t + 1);
        rad *= t;
    }
    uint64_t m = k / rad - 1;
    uint64_t s = sig - k;
    uint64_t d = (s >= rad) ? s - rad : rad - s;
    if (m == 0) return d == 0;
    return d % m == 0;
}

static uint64_t *A336552_terms = 0;
static uint64_t A336552_cnt = 0, A336552_cap = 0, A336552_next = 1;

seq_t a(idx_t n) {
    while (A336552_cnt < (uint64_t)n) {
        uint64_t k = A336552_next++;
        if (A336552_test(k)) {
            if (A336552_cnt == A336552_cap) {
                A336552_cap = A336552_cap ? A336552_cap * 2 : 1024;
                A336552_terms = (uint64_t *)realloc(A336552_terms, A336552_cap * sizeof(uint64_t));
            }
            A336552_terms[A336552_cnt++] = k;
        }
    }
    return (seq_t)A336552_terms[n - 1];
}
