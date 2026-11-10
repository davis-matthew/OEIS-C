/*
 * A325811  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j) where f(n) = [A034460(n), A325814(n)] for all other
 *          numbers, except f(p) = -1 for odd primes.
 * https://oeis.org/A325811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index (by first appearance) of the class of f(n),
 *            f(n)=[A034460(n), A325814(n)], f(p) = -1 for odd primes;
 *            computed by incremental class numbering
 * Range:     u8 n <= 332, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 8319 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *c_ids = NULL;
static seq_t *c_k1 = NULL;
static seq_t *c_k2 = NULL;
static idx_t c_cap = 0;
static idx_t c_done = 0;
static idx_t c_ncls = 0;
static bool c_have_prime = false;
static seq_t c_prime_id = 0;

static void c_grow(idx_t need) {
    if (need <= c_cap) return;
    idx_t nc = c_cap ? c_cap : 1024;
    while (nc < need) nc *= 2;
    seq_t *i2 = mem_new(seq_t, (size_t)nc);
    seq_t *a2 = mem_new(seq_t, (size_t)nc);
    seq_t *b2 = mem_new(seq_t, (size_t)nc);
    for (idx_t i = 0; i < c_done; ++i) i2[i] = c_ids[i];
    for (idx_t i = 0; i < c_ncls; ++i) { a2[i] = c_k1[i]; b2[i] = c_k2[i]; }
    if (c_ids) { mem_free(c_ids); mem_free(c_k1); mem_free(c_k2); }
    c_ids = i2; c_k1 = a2; c_k2 = b2;
    c_cap = nc;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > c_done) {
        c_grow(n);
        for (idx_t m = c_done + 1; m <= n; ++m) {
            idx_t t = m;
            seq_t us = 1, sg = 1;
            int nf = 0;
            bool oddp = false;
            for (idx_t p = 2; p * p <= t; ++p) {
                if (t % p == 0) {
                    seq_t pw = 1, s = 1;
                    while (t % p == 0) {
                        t /= p;
                        pw *= (seq_t)p;
                        s += pw;
                    }
                    us *= (pw + 1);
                    sg *= s;
                    ++nf;
                }
            }
            if (t > 1) {
                seq_t pw = (seq_t)t;
                us *= (pw + 1);
                sg *= (pw + 1);
                ++nf;
                if (nf == 1 && (m & 1) && m > 2) oddp = true;
            }
            seq_t id;
            if (oddp) {
                if (!c_have_prime) {
                    c_have_prime = true;
                    ++c_ncls;
                    c_prime_id = (seq_t)c_ncls;
                    /* placeholder key slot; never matched */
                    c_k1[c_ncls - 1] = 0;
                    c_k2[c_ncls - 1] = 0;
                }
                id = c_prime_id;
            } else {
                seq_t k1 = us - (seq_t)m;
                seq_t k2 = (seq_t)m - (sg - us);
                bool found = false;
                id = 0;
                for (idx_t i = 0; i < c_ncls; ++i) {
                    if (c_have_prime && (seq_t)(i + 1) == c_prime_id) continue;
                    if (c_k1[i] == k1 && c_k2[i] == k2) {
                        id = (seq_t)(i + 1);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    c_k1[c_ncls] = k1;
                    c_k2[c_ncls] = k2;
                    ++c_ncls;
                    id = (seq_t)c_ncls;
                }
            }
            c_ids[m - 1] = id;
        }
        c_done = n;
    }
    return c_ids[n - 1];
}
