/*
 * A350831  a(n) is the least positive integer not listed earlier such that
 *          its first digit together with a(n-1)'s last digit can form a
 *          prime; a(1) = 1.
 * https://oeis.org/A350831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least unused positive integer not a multiple of 10 such
 *            that its first digit together with a(n‑1)’s last digit can form
 *            a prime (either order). a(1) = 1. This definition is implemented
 *            directly by searching from the smallest unused candidate
 * Range:     u8 n <= 57, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A350831: a(1) = 1; a(n) is the least positive integer not yet used whose
 * first digit, with the last digit of a(n-1), forms a prime (in either order:
 * 10x + y or 10y + x). Numbers ending in 0 are never used: no digit forms a
 * prime with 0. Generated in order and cached. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static unsigned char *used = NULL;
static uint64_t ucap = 0, smallest = 1;

static int is_prime2(unsigned v) { if (v < 2) return 0; for (unsigned d = 2; d * d <= v; ++d) if (v % d == 0) return 0; return 1; }
static unsigned first_digit(uint64_t v) { while (v >= 10) v /= 10; return (unsigned)v; }

static void mark(uint64_t v)
{
    if (v >= ucap) {
        uint64_t c = ucap ? ucap : 1024;
        while (c <= v) c *= 2;
        unsigned char *u = (unsigned char *)realloc(used, (size_t)c);
        if (!u) abort();
        for (uint64_t i = ucap; i < c; ++i) u[i] = 0;
        used = u;
        ucap = c;
    }
    used[v] = 1;
}

static int is_used(uint64_t v) { return v < ucap && used[v]; }

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        uint64_t v;
        if (!term_len) v = 1;
        else {
            unsigned x = (unsigned)(terms[term_len - 1] % 10);
            while (is_used(smallest)) ++smallest;
            for (v = smallest;; ++v) {
                if (is_used(v) || v % 10 == 0) continue;    /* a last digit 0 has no successor */
                unsigned y = first_digit(v);
                if (is_prime2(10 * x + y) || is_prime2(10 * y + x)) break;
            }
        }
        mark(v);
        if (term_len == term_cap) {
            term_cap = term_cap ? 2 * term_cap : 1024;
            uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
            if (!t) abort();
            terms = t;
        }
        terms[term_len++] = v;
    }
    return (seq_t)terms[n - 1];
}
