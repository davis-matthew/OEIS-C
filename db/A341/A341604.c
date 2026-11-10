/*
 * A341604  Those primitive elements of A337386 that have exactly one
 *          primitive nondeficient divisor (A006039).
 * https://oeis.org/A341604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that k is a primitive element of A337386
 *            (shift(k) is primitive nondeficient, i.e.
 *            A003973(k)>=2*A003961(k) with no proper divisor having this
 *            property) and A337690(k)=1; computed by a scan with a
 *            smallest-prime-factor sieve
 * Range:     u8 none, u16 n <= 26, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

typedef unsigned __int128 u128_t341604;

/* true if shift(m) (each prime replaced by next prime) is nondeficient */
static bool A341604_shift_nd(const uint32_t *spf, uint64_t m)
{
    u128_t341604 num = 1, den = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        uint64_t q = next_prime(p);
        u128_t341604 qe = 1, sg = 1;
        while (m % p == 0) {
            m /= p;
            qe *= q;
            sg += qe;
        }
        num *= sg;
        den *= qe;
    }
    return num >= 2 * den;
}

/* true if m is nondeficient: sigma(m) >= 2m */
static bool A341604_nd(const uint32_t *spf, uint64_t m)
{
    u128_t341604 num = 1;
    uint64_t orig = m;
    while (m > 1) {
        uint64_t p = spf[m];
        u128_t341604 pe = 1, sg = 1;
        while (m % p == 0) {
            m /= p;
            pe *= p;
            sg += pe;
        }
        num *= sg;
    }
    return num >= (u128_t341604)2 * orig;
}

/* primitive nondeficient (A006039) */
static bool A341604_pnd(const uint32_t *spf, uint64_t d)
{
    if (d < 2 || !A341604_nd(spf, d)) return false;
    uint64_t m = d;
    while (m > 1) {
        uint64_t p = spf[m];
        while (m % p == 0) m /= p;
        if (A341604_nd(spf, d / p)) return false;
    }
    return true;
}

/* is k primitive in A337386 */
static bool A341604_prim(const uint32_t *spf, uint64_t k)
{
    if (!A341604_shift_nd(spf, k)) return false;
    uint64_t m = k;
    while (m > 1) {
        uint64_t p = spf[m];
        while (m % p == 0) m /= p;
        if (A341604_shift_nd(spf, k / p)) return false;
    }
    return true;
}

/* number of primitive nondeficient divisors of k */
static uint64_t A341604_count(const uint32_t *spf, uint64_t k)
{
    static uint64_t divs[16384];
    size_t nd = 1;
    divs[0] = 1;
    uint64_t m = k;
    while (m > 1) {
        uint64_t p = spf[m];
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        size_t cur = nd;
        uint64_t pw = 1;
        for (int i = 1; i <= e; i++) {
            pw *= p;
            for (size_t j = 0; j < cur; j++)
                divs[nd++] = divs[j] * pw;
        }
    }
    uint64_t c = 0;
    for (size_t i = 0; i < nd; i++)
        if (A341604_pnd(spf, divs[i])) c++;
    return c;
}

seq_t a(idx_t n)
{
    static seq_t *res = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t cur = 1;
    static uint64_t lim = 0;
    static uint32_t *spf = NULL;

    while (len < (size_t)n) {
        if (cur > lim) {
            uint64_t nl = lim ? lim * 2 : 1 << 18;
            free(spf);
            spf = (uint32_t *)calloc((size_t)nl + 1, sizeof(uint32_t));
            for (uint64_t i = 2; i <= nl; i++) {
                if (spf[i] == 0) {
                    for (uint64_t j = i; j <= nl; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
            lim = nl;
        }
        uint64_t k = cur++;
        if (k < 2) continue;
        if (A341604_prim(spf, k) && A341604_count(spf, k) == 1) {
            if (len == cap) {
                size_t nc = cap ? cap * 2 : 64;
                res = mem_resize(seq_t, res, nc);
                cap = nc;
            }
            res[len++] = (seq_t)k;
        }
    }
    return res[n - 1];
}
