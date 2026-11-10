/*
 * A299760  Primes p with index k >= 3 such that A288189(k) = A295185(k).
 * https://oeis.org/A299760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = n-th prime p >= 5 with min composite m, sopfr(m)
 *            multiple of p, equal to min composite m with sopfr(m)=p;
 *            sopfr(m)=r*p tested for r=1
 * Range:     u8 n <= 9, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-08)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime.h"

static seq_t *A056240_x = NULL;
static size_t A056240_n = 0;

static void A056240_ensure(size_t t) {
    if (t < A056240_n) return;
    size_t nn = t + 16;
    seq_t *np = mem_new(seq_t, nn);
    for (size_t i = 0; i < A056240_n; ++i) np[i] = A056240_x[i];
    if (A056240_x) mem_free(A056240_x);
    A056240_x = np;
    for (size_t i = A056240_n; i < nn; ++i) {
        if (i < 2) { A056240_x[i] = 0; continue; }
        if (is_prime((idx_t)i)) { A056240_x[i] = (seq_t)i; continue; }
        bool have = false;
        seq_t best = 0;
        for (size_t p = 2; p <= i / 2; ++p) {
            if (!is_prime((idx_t)p)) continue;
            seq_t c = (seq_t)p * A056240_x[i - p];
            if (!have || c < best) { best = c; have = true; }
        }
        A056240_x[i] = best;
    }
    A056240_n = nn;
}

/* min over primes q with t=n-q>=2 of X(t)*q, starting from cap (if have) */
static seq_t min_composite_sopfr(uint64_t n, bool have, seq_t best) {
    for (uint64_t t = 2; t + 2 <= n; ++t) {
        if (have && 2 * t <= n && (seq_t)t * (seq_t)(n - t) >= best) break;
        uint64_t q = n - t;
        if (!is_prime((idx_t)q)) continue;
        A056240_ensure((size_t)t);
        seq_t c = A056240_x[t] * (seq_t)q;
        if (!have || c < best) { best = c; have = true; }
    }
    return best;
}

static bool in_seq(uint64_t p) {
    seq_t B = min_composite_sopfr(p, false, 0);
    for (uint64_t r = 2; ; ++r) {
        seq_t lb = (seq_t)2 * ((seq_t)r * (seq_t)p - 2);
        if (lb >= B) break;
        seq_t c = min_composite_sopfr(r * p, true, B);
        if (c < B) return false;
    }
    return true;
}

static uint64_t *terms = NULL;
static size_t terms_cnt = 0, terms_cap = 0;
static uint64_t next_p = 5;

seq_t a(idx_t n) {
    while (terms_cnt < (size_t)n) {
        uint64_t p = next_p;
        next_p += 2;
        if (!is_prime((idx_t)p)) continue;
        if (!in_seq(p)) continue;
        if (terms_cnt == terms_cap) {
            size_t nc = terms_cap ? terms_cap * 2 : 64;
            uint64_t *nt = (uint64_t *)malloc(nc * sizeof(uint64_t));
            for (size_t i = 0; i < terms_cnt; ++i) nt[i] = terms[i];
            if (terms) free(terms);
            terms = nt;
            terms_cap = nc;
        }
        terms[terms_cnt++] = p;
    }
    return (seq_t)terms[n - 1];
}
