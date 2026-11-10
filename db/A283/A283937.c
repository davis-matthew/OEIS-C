/*
 * A283937  Prime numbers p such that A283751(p) < p.
 * https://oeis.org/A283937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. a(n) = n-th prime p for which some base b with 2 <= b <=
 *            sqrt(p) gives a base-b digit string of p that, read in base b^2,
 *            is divisible by p. No base in (sqrt(p), p) can work: there p =
 *            d1*b + d0 and the value d1*b^2 + d0 == d1*b*(b-1) (mod p), which
 *            is not 0 mod p. Base b = p always works...
 * Range:     u8 n <= 10, u16 n <= 82, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 62 terms through n = 188 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A283937_mm(uint64_t a, uint64_t b, uint64_t m) {
    if (m <= 0xFFFFFFFFULL) return (a * b) % m;
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

/* true if some base b with b*b < p works for p */
static bool A283937_smaller(uint64_t p) {
    for (uint64_t b = 2; b * b < p; ++b) {
        uint64_t t = p, mod = 0, pw = 1 % p;
        uint64_t mult = A283937_mm(b, b, p);
        while (t) {
            uint64_t d = t % b;
            t /= b;
            mod = (mod + A283937_mm(d, pw, p)) % p;
            pw = A283937_mm(pw, mult, p);
        }
        if (mod == 0) return true;
    }
    return false;
}

static uint64_t *A283937_terms = 0;
static size_t A283937_cnt = 0, A283937_cap = 0;
static uint64_t A283937_next = 2;

seq_t a(idx_t n) {
    while (A283937_cnt < (size_t)n) {
        uint64_t p = A283937_next++;
        if (!is_prime((idx_t)p)) continue;
        if (A283937_smaller(p)) {
            if (A283937_cnt == A283937_cap) {
                size_t nc = A283937_cap ? A283937_cap * 2 : 256;
                uint64_t *nt = (uint64_t *)realloc(A283937_terms, nc * sizeof(uint64_t));
                if (!nt) return 0;
                A283937_terms = nt;
                A283937_cap = nc;
            }
            A283937_terms[A283937_cnt++] = p;
        }
    }
    return (seq_t)A283937_terms[n - 1];
}
