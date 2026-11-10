/*
 * A397934  Numbers that are neither squarefree, powerful, nor in either
 *          A396594 or A397221.
 * https://oeis.org/A397934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with rad(k)!=k, rad(k)^2 !| k, k!=rad(k)*omega(k),
 *            k!=rad(k)*bigomega(k); found by search
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 7910 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A397934_terms = NULL;
static size_t A397934_cnt = 0, A397934_cap = 0;
static uint64_t A397934_last = 1;

static int A397934_ok(uint64_t k)
{
    uint64_t m = k, rad = 1, om = 0, big = 0, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            rad *= p;
            om++;
            while (m % p == 0) { m /= p; big++; }
        }
    }
    if (m > 1) { rad *= m; om++; big++; }
    if (rad == k) return 0;
    if (k % (rad * rad) == 0) return 0;
    if (rad * om == k) return 0;
    if (rad * big == k) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    while (A397934_cnt < (size_t)n) {
        A397934_last++;
        if (A397934_ok(A397934_last)) {
            if (A397934_cnt == A397934_cap) {
                size_t nc = A397934_cap ? A397934_cap * 2 : 1024;
                uint64_t *t = (uint64_t *)realloc(A397934_terms, nc * sizeof(uint64_t));
                if (!t) return 0;
                A397934_terms = t;
                A397934_cap = nc;
            }
            A397934_terms[A397934_cnt++] = A397934_last;
        }
    }
    return (seq_t)A397934_terms[n - 1];
}
