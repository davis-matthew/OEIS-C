/*
 * A325520  Difference sequence of A325427.
 * https://oeis.org/A325520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325427(n+1) - A325427(n), where A325427(n) is the least
 *            number not equal to 2*A325427(m)+1 or floor(3*A325427(m)/2) for
 *            m < n (computed by a sieve)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9475 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *s_terms = NULL;
static size_t s_len = 0, s_cap = 0;
static unsigned char *s_forb = NULL;
static size_t s_fcap = 0;
static uint64_t s_next = 1;

static void s_mark(uint64_t x)
{
    if (x >= s_fcap) {
        size_t nc = s_fcap ? s_fcap : 64;
        while (nc <= x) nc <<= 1;
        unsigned char *t = (unsigned char *)realloc(s_forb, nc);
        if (!t) abort();
        memset(t + s_fcap, 0, nc - s_fcap);
        s_forb = t;
        s_fcap = nc;
    }
    s_forb[x] = 1;
}

static void s_gen(size_t need)
{
    if (need > s_cap) {
        size_t nc = s_cap ? s_cap : 64;
        while (nc < need) nc <<= 1;
        uint64_t *t = (uint64_t *)realloc(s_terms, nc * sizeof(uint64_t));
        if (!t) abort();
        s_terms = t;
        s_cap = nc;
    }
    while (s_len < need) {
        while (s_next < s_fcap && s_forb[s_next]) s_next++;
        uint64_t v = s_next;
        s_terms[s_len++] = v;
        s_mark(2 * v + 1);
        s_mark((v * 3) / 2);
        s_next++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    s_gen((size_t)n + 1);
    return (seq_t)(s_terms[n] - s_terms[n - 1]);
}
