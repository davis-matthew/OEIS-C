/*
 * A323167  a(n) = A294898(A122111(n)).
 * https://oeis.org/A323167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A294898(A122111(n)) = 2*m - sigma(m) - A000120(m), where
 *            m = A122111(n) is built from the conjugate of the prime-index
 *            partition of n. A000120(m) is computed exactly with a small
 *            multi-limb integer, because m can be far larger than seq_t
 * Range:     i8 n <= 37, i16 n <= 105, i32 n <= 261, i64 n <= 621,
 *            i128 n <= 1437, big any n
 * Validated: standard, pass, 97 terms through n = 9521 (2026-11-08)
 * Helpers:   factor_u64, nth_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/nth_prime.h"

static int cmp_idx_323167(const void *x, const void *y)
{
    idx_t va = *(const idx_t *)x;
    idx_t vb = *(const idx_t *)y;
    return (va > vb) - (va < vb);
}

/* multiply little-endian uint32 limb array by small factor p */
static void big_mul_small_323167(uint32_t **limbs, size_t *len, size_t *cap, uint32_t p)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t t = (uint64_t)(*limbs)[i] * p + carry;
        (*limbs)[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        if (*len == *cap) {
            *cap *= 2;
            *limbs = (uint32_t *)realloc(*limbs, *cap * sizeof(uint32_t));
            if (!*limbs) abort();
        }
        (*limbs)[(*len)++] = (uint32_t)carry;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t m = 1;
    seq_t sig = 1;

    size_t cap = 16, len = 1;
    uint32_t *limbs = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limbs) abort();
    limbs[0] = 1;

    if (n > 1) {
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt = 0;
        factor_u64(n, primes, exps, &pcnt);

        idx_t indices[128];
        size_t cnt = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            idx_t idx = (idx_t)prime_pi(primes[i]);
            for (uint64_t e = 0; e < exps[i]; ++e) indices[cnt++] = idx;
        }
        qsort(indices, cnt, sizeof(idx_t), cmp_idx_323167);

        idx_t remaining = (idx_t)cnt;
        idx_t last_part = 1;
        size_t pos = 0;
        while (pos < cnt) {
            idx_t current = indices[pos];
            size_t freq = 0;
            while (pos < cnt && indices[pos] == current) { ++freq; ++pos; }
            idx_t length = current - last_part + 1;
            uint64_t pu = nth_prime(remaining);
            seq_t p = (seq_t)pu;
            seq_t sum = 1, pw = 1;
            for (idx_t k = 0; k < length; ++k) {
                pw *= p;
                sum += pw;
                big_mul_small_323167(&limbs, &len, &cap, (uint32_t)pu);
            }
            m *= pw;
            sig *= sum;
            remaining -= (idx_t)freq;
            last_part = current + 1;
        }
    }

    seq_t pc = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t v = limbs[i];
        while (v) { v &= v - 1; pc++; }
    }
    free(limbs);
    return 2 * m - sig - pc;
}
