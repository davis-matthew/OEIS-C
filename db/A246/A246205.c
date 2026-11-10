/*
 * A246205  Permutation of natural numbers: a(1) = 1, a(A014580(n)) =
 *          A117968(a(n)), a(A091242(n)) = A117967(1+a(n)), where A117967 and
 *          A117968 give positive and negative parts of inverse of balanced
 *          ternary enumeration of integers, and A014580 resp. A091242 are the
 *          binary coded irreducible resp. reducible p...
 * https://oeis.org/A246205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, if n is irreducible over GF(2) (n in A014580, n
 *            = A014580(k)) then a(n) = A117968(a(k)); otherwise (n =
 *            A091242(m)) a(n) = A117967(1+a(m)). Computed by a sieve of
 *            carryless products plus a running table
 * Range:     u8 n <= 28, u16 n <= 103, u32 n <= 576, u64 n <= 6094,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 92 terms through n = 9325 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A117968_a(idx_t n)
{
    seq_t r = 0;
    /* iterative form of the recurrence: a(1)=2, a(3n)=3a(n), a(3n+1)=3a(n)+2, a(3n-1)=3a(n)+1 */
    /* collect digits */
    int ops[64]; int cnt = 0;
    while (n != 1) {
        int m = (int)(n % 3);
        if (m == 0) { ops[cnt++] = 0; n = n / 3; }
        else if (m == 1) { ops[cnt++] = 2; n = (n - 1) / 3; }
        else { ops[cnt++] = 1; n = (n + 1) / 3; }
    }
    r = (seq_t)2;
    while (cnt > 0) {
        int o = ops[--cnt];
        r = r * (seq_t)3 + (seq_t)o;
    }
    return r;
}

static seq_t A117967_a(seq_t nn)
{
    /* a(0)=0, a(3n)=3a(n), a(3n+1)=3a(n)+1, a(3n-1)=3a(n)+2; nn is seq_t, may be large */
    int ops[200]; int cnt = 0;
    seq_t n = nn;
    while (n != 0) {
        int m = (int)(n % (seq_t)3);
        if (m == 0) { ops[cnt++] = 0; n = n / (seq_t)3; }
        else if (m == 1) { ops[cnt++] = 1; n = (n - (seq_t)1) / (seq_t)3; }
        else { ops[cnt++] = 2; n = (n + (seq_t)1) / (seq_t)3; }
    }
    seq_t r = 0;
    while (cnt > 0) {
        int o = ops[--cnt];
        r = r * (seq_t)3 + (seq_t)o;
    }
    return r;
}

static seq_t *tab = NULL;
static idx_t tab_n = 0;

static unsigned blen64(uint64_t x)
{
    unsigned b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static void build(idx_t N)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    unsigned dN = blen64((uint64_t)N);
    for (uint64_t p = 2; p <= N; p++) {
        unsigned dp = blen64(p);
        if (2 * dp - 1 > dN) break;
        uint64_t qmax = ((uint64_t)1 << (dN - dp + 1)) - 1;
        if (qmax > N) qmax = N;
        for (uint64_t q = p; q <= qmax; q++) {
            uint64_t prod = 0, a = q, b = p;
            while (b) {
                if (b & 1) prod ^= a;
                a <<= 1;
                b >>= 1;
            }
            if (prod <= N) comp[prod] = 1;
        }
    }
    if (tab) mem_free(tab);
    tab = mem_new(seq_t, (size_t)N + 1);
    tab[1] = (seq_t)1;
    idx_t irr = 0, red = 0;
    for (idx_t k = 2; k <= N; k++) {
        if (!comp[k]) {
            irr++;
            tab[k] = A117968_a((idx_t)tab[irr]);
        } else {
            red++;
            tab[k] = A117967_a((seq_t)1 + tab[red]);
        }
    }
    free(comp);
    tab_n = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > tab_n) {
        idx_t N = n < 1024 ? 1024 : n;
        build(N);
    }
    return tab[n];
}
