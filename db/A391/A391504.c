/*
 * A391504  Coreful-infinitary abundant numbers: numbers k for which
 *          A363331(k) > 2*k.
 * https://oeis.org/A391504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A363331(k) > 2*k, where A363331 is
 *            multiplicative with A363331(p^e) = (Product over bits 2^j of e
 *            of (1 + p^(2^j))) - 1; found by sieve search
 * Range:     u8 none, u16 n <= 188, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9725 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint32_t *A391504_spf = NULL;
static uint64_t A391504_lim = 0;
static seq_t *A391504_terms = NULL;
static size_t A391504_len = 0, A391504_cap = 0;
static uint64_t A391504_next = 2;

static void A391504_build(uint64_t lim)
{
    if (A391504_spf) free(A391504_spf);
    A391504_spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    for (uint64_t i = 2; i <= lim; i++) {
        if (A391504_spf[i] == 0) {
            for (uint64_t j = i; j <= lim; j += i)
                if (A391504_spf[j] == 0) A391504_spf[j] = (uint32_t)i;
        }
    }
    A391504_lim = lim;
}

static seq_t A391504_csig(uint64_t k)
{
    seq_t prod = 1;
    uint64_t m = k;
    while (m > 1) {
        uint64_t p = A391504_spf[m];
        unsigned e = 0;
        while (m % p == 0) { m /= p; e++; }
        seq_t term = 1;
        seq_t pp = (seq_t)p;
        while (e > 0) {
            if (e & 1) term = term * (1 + pp);
            e >>= 1;
            if (e) pp = pp * pp;
        }
        prod = prod * (term - 1);
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A391504_cap == 0) {
        A391504_cap = 256;
        A391504_terms = mem_new(seq_t, A391504_cap);
    }
    while (A391504_len < (size_t)n) {
        if (A391504_next > A391504_lim)
            A391504_build(A391504_lim ? A391504_lim * 2 : (1ULL << 20));
        uint64_t k = A391504_next++;
        seq_t s = A391504_csig(k);
        if (s > (seq_t)2 * (seq_t)k) {
            if (A391504_len >= A391504_cap) {
                A391504_cap *= 2;
                A391504_terms = mem_resize(seq_t, A391504_terms, A391504_cap);
            }
            A391504_terms[A391504_len++] = (seq_t)k;
        }
    }
    return A391504_terms[(size_t)(n - 1)];
}
