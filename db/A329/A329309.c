/*
 * A329309  a(n) is the first prime p such that A329308(p) = n.
 * https://oeis.org/A329309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p in increasing order; a(n) = first prime p
 *            with #{k : 1 < k < sqrt(p), p mod k^2 is prime} = n. Primality
 *            comes from an incrementally grown sieve, and all first
 *            occurrences are recorded in one sweep
 * Range:     u8 n <= 9, u16 n <= 78, u32 n <= 325, u64 n <= 325,
 *            u128 n <= 325, big any n
 * Validated: standard, pass, 68 terms through n = 316 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *A329309_comp = NULL; /* composite flags, 0..A329309_lim */
static idx_t A329309_lim = 0;
static idx_t A329309_done = 1;             /* all p <= done processed */
static idx_t *A329309_first = NULL;        /* first[c] = least prime with count c, 0 = unknown */
static size_t A329309_cap = 0;

static void A329309_sieve(idx_t lim)
{
    unsigned char *c = (unsigned char *)malloc((size_t)lim + 1);
    memset(c, 0, (size_t)lim + 1);
    c[0] = 1;
    if (lim >= 1) c[1] = 1;
    for (idx_t i = 2; i * i <= lim; ++i)
        if (!c[i])
            for (idx_t j = i * i; j <= lim; j += i)
                c[j] = 1;
    free(A329309_comp);
    A329309_comp = c;
    A329309_lim = lim;
}

static void A329309_record(idx_t cnt, idx_t p)
{
    if ((size_t)cnt >= A329309_cap) {
        size_t nc = A329309_cap ? A329309_cap : 64;
        while (nc <= (size_t)cnt) nc *= 2;
        idx_t *f = (idx_t *)realloc(A329309_first, nc * sizeof(idx_t));
        for (size_t i = A329309_cap; i < nc; ++i) f[i] = 0;
        A329309_first = f;
        A329309_cap = nc;
    }
    if (A329309_first[cnt] == 0)
        A329309_first[cnt] = p;
}

static void A329309_extend(void)
{
    idx_t nl = A329309_lim ? A329309_lim * 2 : ((idx_t)1 << 16);
    A329309_sieve(nl);
    const unsigned char *c = A329309_comp;
    for (idx_t p = A329309_done + 1; p <= nl; ++p) {
        if (c[p]) continue;
        idx_t cnt = 0;
        for (idx_t k = 2; k * k < p; ++k)
            if (!c[p % (k * k)])
                ++cnt;
        A329309_record(cnt, p);
    }
    A329309_done = nl;
}

seq_t a(idx_t n)
{
    for (;;) {
        if ((size_t)n < A329309_cap && A329309_first[n] != 0)
            return (seq_t)A329309_first[n];
        A329309_extend();
    }
}
