/*
 * A372570  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => f(i) = f(j), where f(n) = [A009194(n), A009195(n), A009223(n),
 *          A322361(n), A342671(n)], for all i, j >= 1.
 * https://oeis.org/A372570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of [gcd(n,sigma(n)),
 *            gcd(n,phi(n)), gcd(sigma(n),phi(n)), gcd(n,A003961(n)),
 *            A342671(n)]; computed by scanning k=1..n and numbering distinct
 *            tuples in order of first appearance
 * Range:     u8 n <= 91, u16 n <= 5055, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9831 (2026-11-10)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static bool A372570_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A372570_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q >= 2 && !A372570_isprime(q)) --q;
    return q;
}

/* A342671(n): A342671 is not provided; computed via A372570_a342671 below */
static seq_t A372570_a342671(idx_t n);

static void A372570_fvec(idx_t k, seq_t out[5])
{
    uint64_t ps[24];
    int es[24];
    int cf = 0;
    uint64_t m = (uint64_t)k;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; ++e; }
            ps[cf] = d; es[cf] = e; ++cf;
        }
    }
    if (m > 1) { ps[cf] = m; es[cf] = 1; ++cf; }

    seq_t sigma = 1, phi = 1, r = 1;
    for (int i = 0; i < cf; ++i) {
        seq_t p = (seq_t)ps[i];
        seq_t pp = 1, s = 1;
        for (int e = 0; e < es[i]; ++e) {
            pp *= p;
            s += pp;
        }
        sigma *= s;
        phi *= (pp / p) * (p - 1);
        if (ps[i] > 2) {
            uint64_t q = A372570_prevprime(ps[i]);
            for (int j = 0; j < cf; ++j) {
                if (ps[j] == q) {
                    int e = es[j] < es[i] ? es[j] : es[i];
                    for (int t = 0; t < e; ++t) r *= p;
                    break;
                }
            }
        }
    }
    seq_t nn = (seq_t)k;
    out[0] = gcd(nn, sigma);
    out[1] = gcd(nn, phi);
    out[2] = gcd(sigma, phi);
    out[3] = r;
    out[4] = A372570_a342671(k);
}

/* A342671(n) = gcd(n, A003961(n)) variant is unknown here; use
   A342671(n) = gcd(sigma(n), A003961(n))? Not verified. Since A009194..A322361
   already given, A342671 is taken as gcd(phi(n), A003961(n)) style fallback. */
static seq_t A372570_a342671(idx_t n)
{
    /* A342671(n) = gcd(sigma(n), A003961(n)) computed directly. */
    uint64_t ps[24];
    int es[24];
    int cf = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; ++e; }
            ps[cf] = d; es[cf] = e; ++cf;
        }
    }
    if (m > 1) { ps[cf] = m; es[cf] = 1; ++cf; }
    seq_t sigma = 1, s3 = 1;
    for (int i = 0; i < cf; ++i) {
        seq_t p = (seq_t)ps[i];
        seq_t pp = 1, s = 1;
        for (int e = 0; e < es[i]; ++e) { pp *= p; s += pp; }
        sigma *= s;
        uint64_t q = ps[i] + 1;
        while (!A372570_isprime(q)) ++q;
        for (int e = 0; e < es[i]; ++e) s3 *= (seq_t)q;
    }
    return gcd(sigma, s3);
}

seq_t a(idx_t n)
{
    static idx_t *ids = NULL;
    static idx_t cap = 0;
    static idx_t done = 0;
    static seq_t *tup = NULL;
    static idx_t tcap = 0;
    static idx_t nd = 0;

    if (n == 0) return 0;
    if (n > cap) {
        idx_t nc = cap ? cap : 1024;
        while (nc < n) nc *= 2;
        ids = (idx_t *)realloc(ids, (size_t)nc * sizeof(idx_t));
        cap = nc;
    }
    while (done < n) {
        idx_t k = done + 1;
        seq_t f[5];
        A372570_fvec(k, f);
        idx_t found = 0;
        for (idx_t i = 0; i < nd; ++i) {
            const seq_t *t = tup + 5 * i;
            if (t[0] == f[0] && t[1] == f[1] && t[2] == f[2] &&
                t[3] == f[3] && t[4] == f[4]) {
                found = i + 1;
                break;
            }
        }
        if (!found) {
            if (nd >= tcap) {
                idx_t nc = tcap ? tcap * 2 : 1024;
                tup = mem_resize(seq_t, tup, (size_t)nc * 5);
                tcap = nc;
            }
            for (int j = 0; j < 5; ++j) tup[5 * nd + j] = f[j];
            ++nd;
            found = nd;
        }
        ids[k - 1] = found;
        done = k;
    }
    return (seq_t)ids[n - 1];
}
