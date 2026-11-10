/*
 * A379479  Numbers k such that the greatest common divisor of k, sigma(k) and
 *          A003961(k) is 1 and gcd(A003961(k)-2k, A003961(k)-sigma(k)) > 1.
 * https://oeis.org/A379479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with gcd(k, sigma(k), A003961(k)) = 1 and
 *            gcd(A003961(k)-2k, A003961(k)-sigma(k)) > 1; found by sequential
 *            search with a cache
 * Range:     u8 n <= 19, u16 n <= 894, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 84 terms through n = 9665 (2026-11-10)
 * Helpers:   gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

static idx_t *qual_cache = NULL;
static size_t qual_len = 0;
static size_t qual_cap = 0;
static idx_t qual_next = 1;

static bool qual_test(idx_t k)
{
    uint64_t m = (uint64_t)k;
    seq_t sig = 1;
    seq_t b = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t pp = 1, term = 1, s = 1;
            while (m % p == 0) {
                m /= p;
                pp *= (seq_t)p;
                s += pp;
                term *= (seq_t)q;
            }
            sig *= s;
            b *= term;
        }
    }
    if (m > 1) {
        sig *= ((seq_t)m + 1);
        b *= (seq_t)next_prime(m);
    }
    seq_t g = gcd((seq_t)k, sig);
    g = gcd(g, b);
    if (g != 1) return false;
    seq_t two_k = (seq_t)k * 2;
    seq_t d1 = (b > two_k) ? (b - two_k) : (two_k - b);
    seq_t d2 = (b > sig) ? (b - sig) : (sig - b);
    seq_t h = gcd(d1, d2);
    return h > 1;
}

seq_t a(idx_t n)
{
    while (qual_len < (size_t)n) {
        if (qual_test(qual_next)) {
            if (qual_len == qual_cap) {
                size_t nc = qual_cap ? qual_cap * 2 : 256;
                idx_t *t = (idx_t *)realloc(qual_cache, nc * sizeof(idx_t));
                if (!t) abort();
                qual_cache = t;
                qual_cap = nc;
            }
            qual_cache[qual_len++] = qual_next;
        }
        qual_next++;
    }
    return (seq_t)qual_cache[n - 1];
}
