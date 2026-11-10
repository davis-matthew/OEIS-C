/*
 * A373373  Numbers k for which both A001414(k) and A276085(k) are multiples
 *          of 3, where A001414 is the sum of prime factors with repetition
 *          and A276085 is the primorial base log-function.
 * https://oeis.org/A373373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A001414(k) == 0 (mod 3) and A276085(k) == 0
 *            (mod 3); found by a scan, using A276085(k) == e2 + 2*e3 (mod 3),
 *            where e2, e3 are the exponents of 2 and 3 in k
 * Range:     u8 n <= 26, u16 n <= 9394, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9605 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A373373_terms = NULL;
static uint64_t A373373_cnt = 0, A373373_cap = 0, A373373_last = 0;

static int A373373_ok(uint64_t k)
{
    uint64_t s1 = 0, e2 = 0, e3 = 0;
    while (k % 2 == 0) { k /= 2; e2++; }
    while (k % 3 == 0) { k /= 3; e3++; }
    s1 = (2 * e2 + 3 * e3) % 3;
    for (uint64_t p = 5; p * p <= k; p += 2) {
        while (k % p == 0) { k /= p; s1 += p % 3; }
    }
    if (k > 1) s1 += k % 3;
    if (s1 % 3 != 0) return 0;
    return (e2 + 2 * e3) % 3 == 0;
}

seq_t a(idx_t n)
{
    while (A373373_cnt < (uint64_t)n) {
        A373373_last++;
        if (A373373_ok(A373373_last)) {
            if (A373373_cnt == A373373_cap) {
                A373373_cap = A373373_cap ? A373373_cap * 2 : 1024;
                A373373_terms = (uint64_t *)realloc(A373373_terms, A373373_cap * sizeof(uint64_t));
            }
            A373373_terms[A373373_cnt++] = A373373_last;
        }
    }
    return (seq_t)A373373_terms[n - 1];
}
