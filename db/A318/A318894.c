/*
 * A318894  Positions of terms > 1 in A205007, or equally, where A205006(n) !=
 *          n.
 * https://oeis.org/A318894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m with A205006(m) != m, i.e. the first
 *            repeated residue of triangular numbers mod m gives T(k)-T(j) !=
 *            m
 * Range:     u8 n <= 10, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *s_stamp = 0;
static uint64_t *s_first = 0;
static uint64_t s_cap = 0;
static uint64_t s_gen = 0;

static uint64_t *s_terms = 0;
static uint64_t s_nterms = 0;
static uint64_t s_tcap = 0;
static uint64_t s_scanned = 0;

static int A318894_is_term(uint64_t m)
{
    if (m > s_cap) {
        uint64_t nc = s_cap ? s_cap : 1024;
        while (nc < m) nc *= 2;
        free(s_stamp);
        free(s_first);
        s_stamp = (uint64_t *)calloc((size_t)nc, sizeof(uint64_t));
        s_first = (uint64_t *)calloc((size_t)nc, sizeof(uint64_t));
        if (!s_stamp || !s_first) return 0;
        s_cap = nc;
    }
    s_gen++;
    for (uint64_t k = 1; ; ++k) {
        uint64_t r = (k * (k + 1) / 2) % m;
        if (s_stamp[r] == s_gen) {
            uint64_t j = s_first[r];
            uint64_t diff = k * (k + 1) / 2 - j * (j + 1) / 2;
            return diff != m;
        }
        s_stamp[r] = s_gen;
        s_first[r] = k;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (s_nterms < (uint64_t)n) {
        s_scanned++;
        if (A318894_is_term(s_scanned)) {
            if (s_nterms == s_tcap) {
                s_tcap = s_tcap ? s_tcap * 2 : 64;
                s_terms = (uint64_t *)realloc(s_terms, (size_t)s_tcap * sizeof(uint64_t));
            }
            s_terms[s_nterms++] = s_scanned;
        }
    }
    return (seq_t)s_terms[n - 1];
}
