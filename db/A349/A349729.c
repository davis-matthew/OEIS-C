/*
 * A349729  Numbers k >= 1 such that A018804(k) + A000203(k) is a triangular
 *          number.
 * https://oeis.org/A349729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff 8*(A018804(k) + A000203(k)) + 1 is a
 *            perfect square, with A018804(p^e) = p^(e-1)*((e+1)*p - e) and
 *            A000203(p^e) = (p^(e+1)-1)/(p-1) (both multiplicative)
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A349729_isqrt(uint64_t x)
{
    uint64_t r = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

static bool A349729_test(uint64_t k)
{
    uint64_t pil = 1, sig = 1;
    uint64_t m = k, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t e = 0, pw = 1, term = 1, pe1 = 1;
            while (m % p == 0) {
                m /= p;
                e++;
                pe1 = pw;
                pw *= p;
                term += pw;
            }
            sig *= term;
            pil *= pe1 * ((e + 1) * p - e);
        }
    }
    if (m > 1) {
        sig *= (m + 1);
        pil *= (2 * m - 1);
    }
    uint64_t x = (pil + sig) * 8 + 1;
    uint64_t r = A349729_isqrt(x);
    return r * r == x;
}

static uint64_t *A349729_terms = 0;
static size_t A349729_cnt = 0, A349729_cap = 0;
static uint64_t A349729_next = 1;

seq_t a(idx_t n)
{
    while (A349729_cnt < (size_t)n) {
        uint64_t k = A349729_next++;
        if (A349729_test(k)) {
            if (A349729_cnt == A349729_cap) {
                A349729_cap = A349729_cap ? A349729_cap * 2 : 64;
                A349729_terms = (uint64_t *)realloc(A349729_terms, A349729_cap * sizeof(uint64_t));
            }
            A349729_terms[A349729_cnt++] = k;
        }
    }
    return (seq_t)A349729_terms[n - 1];
}
