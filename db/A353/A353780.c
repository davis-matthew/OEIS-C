/*
 * A353780  a(1) = 1; for n > 1, a(n) is the smallest positive number that has
 *          not yet appeared that is coprime to a(n-1), does not equal
 *          a(n-1)+1, and has no digit in common with a(n-1).
 * https://oeis.org/A353780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   recurrence-based sequence a(1) = 1; For n > 1, a(n) is the
 *            smallest positive integer that 1) has not yet appeared, 2) is
 *            coprime to a(n‑1), 3) is not a(n‑1)+1, 4) shares no decimal
 *            digit with a(n‑1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A353780: a(1) = 1; a(n) is the smallest number not yet used that is coprime
 * to a(n-1), is not a(n-1) + 1, and shares no digit with a(n-1). Generated in
 * order and cached. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static unsigned char *used = NULL;
static uint64_t ucap = 0, smallest = 1;

static uint64_t gcd64(uint64_t a, uint64_t b) { while (b) { uint64_t t = a % b; a = b; b = t; } return a; }
static unsigned digits(uint64_t v) { unsigned m = 0; do { m |= 1u << (v % 10); v /= 10; } while (v); return m; }
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
            uint64_t p = terms[term_len - 1];
            unsigned dp = digits(p);
            while (is_used(smallest)) ++smallest;
            for (v = smallest;; ++v)
                if (!is_used(v) && v != p + 1 && gcd64(v, p) == 1 && !(digits(v) & dp)) break;
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
