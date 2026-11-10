/*
 * A322863  Permutation of natural numbers: a(0) = 1; for n >= 1, a(n) =
 *          A005940(1+A122111(n)).
 * https://oeis.org/A322863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = A005940(1+A122111(n)), where
 *            A122111(n) is computed exactly as a multi-precision integer (it
 *            can far exceed 64 bits, e.g. A122111(prime(k)) = 2^k) and
 *            A005940(1+m) is read off the binary expansion of m
 * Range:     u8 n <= 33, u16 n <= 113, u32 n <= 582, u64 n <= 2076,
 *            u128 n <= 10540, big any n
 * Validated: standard, pass, 96 terms through n = 9792 (2026-11-08)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/prime_pi.h"

static uint64_t A322863_prime(idx_t k)
{
    static uint64_t *pr = NULL;
    static size_t sz = 0, cap = 0;
    if (cap == 0) {
        cap = 64;
        pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!pr) abort();
        pr[sz++] = 2;
    }
    while (sz < (size_t)k) {
        uint64_t c = pr[sz - 1] + 1;
        for (;; c++) {
            bool ok = true;
            for (size_t i = 0; i < sz; i++) {
                if (pr[i] * pr[i] > c) break;
                if (c % pr[i] == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            if (!pr) abort();
        }
        pr[sz++] = c;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* prime-factor indices in ascending order, with multiplicity */
    idx_t ind[128];
    size_t cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            idx_t pi = (idx_t)prime_pi(p);
            while (t % p == 0) { t /= p; ind[cnt++] = pi; }
        }
    }
    if (t > 1) ind[cnt++] = (idx_t)prime_pi(t);
    if (cnt == 0) return (seq_t)2; /* n = 1: A122111(1) = 1, A005940(2) = 2 */

    /* m = A122111(n) as a little-endian bignum of 32-bit limbs */
    idx_t mx = ind[cnt - 1];
    size_t cap = (size_t)mx * 2 + 4;
    uint32_t *limb = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!limb) abort();
    memset(limb, 0, cap * sizeof(uint32_t));
    limb[0] = 1;
    size_t len = 1;
    size_t pos = 0;
    for (idx_t j = 1; j <= mx; j++) {
        while (pos < cnt && ind[pos] < j) pos++;
        uint64_t q = A322863_prime((idx_t)(cnt - pos)); /* small prime */
        uint64_t carry = 0;
        for (size_t k = 0; k < len; k++) {
            uint64_t v = (uint64_t)limb[k] * q + carry;
            limb[k] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) {
            limb[len++] = (uint32_t)carry;
            carry >>= 32;
        }
    }

    /* A005940(1 + m): scan bits of m from least significant */
    seq_t result = 1;
    idx_t i = 1;
    size_t nbits = len * 32;
    while (nbits > 0 && ((limb[(nbits - 1) / 32] >> ((nbits - 1) % 32)) & 1u) == 0)
        nbits--;
    for (size_t b = 0; b < nbits; b++) {
        if ((limb[b / 32] >> (b % 32)) & 1u)
            result *= (seq_t)A322863_prime(i);
        else
            i++;
    }
    free(limb);
    return result;
}
