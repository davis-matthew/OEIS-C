/*
 * A343405  Numbers k such that A343404(k) = k.
 * https://oeis.org/A343405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search over the primorial-base tree: k (with w primorial digits)
 *            is a fixed point of A343404 iff its top digit d satisfies k' +
 *            d*P_{w-1} == d (mod prime(w)), where k' (k without its top
 *            digit) is itself a fixed point; children are generated level by
 *            level, sorted, and listed in increasing order....
 * Range:     u8 n <= 21, u16 n <= 60, u32 n <= 130, u64 n <= 249,
 *            u128 n <= 466, big any n
 * Validated: standard, pass, 66 terms through n = 961 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A343405_shell(seq_t *x, size_t lo, size_t hi)
{
    size_t len = hi - lo, gap, i, j;
    for (gap = len / 2; gap > 0; gap /= 2) {
        for (i = lo + gap; i < hi; ++i) {
            seq_t t = x[i];
            for (j = i; j >= lo + gap && x[j - gap] > t; j -= gap)
                x[j] = x[j - gap];
            x[j] = t;
        }
    }
}

static void A343405_push(seq_t **arr, size_t *cnt, size_t *cap, seq_t v)
{
    if (*cnt >= *cap) {
        size_t nc = (*cap) * 2 + 16;
        *arr = mem_resize(seq_t, *arr, nc);
        *cap = nc;
    }
    (*arr)[(*cnt)++] = v;
}

/* k = kp + d*P, returning false on overflow of seq_t */
static bool A343405_mk(seq_t kp, unsigned d, seq_t P, seq_t *out)
{
    if (d == 0) { *out = kp; return true; }
    seq_t q = (seq_t)d * P;
    if (q / P != (seq_t)d) return false;
    seq_t k = kp + q;
    if (k < q) return false;
    *out = k;
    return true;
}

seq_t a(idx_t n)
{
    unsigned primes[80];
    int np = 0;
    unsigned c;
    for (c = 2; np < 80; ++c) {
        bool ok = true;
        for (int j = 0; j < np; ++j) {
            if ((unsigned)primes[j] * primes[j] > c) break;
            if (c % primes[j] == 0) { ok = false; break; }
        }
        if (ok) primes[np++] = c;
    }

    if (n <= 1) return (seq_t)0;

    size_t tcnt = 0, tcap = 16;
    seq_t *terms = mem_new(seq_t, tcap);
    size_t scnt = 0, scap = 16;
    seq_t *S = mem_new(seq_t, scap);
    seq_t result = (seq_t)0;

    terms[tcnt++] = (seq_t)0;
    S[scnt++] = (seq_t)0;
    seq_t P = (seq_t)1;
    bool pover = false;

    for (int w = 0; w < np; ++w) {
        if (pover) break;
        unsigned p = primes[w];
        uint64_t cm = 1;
        for (int j = 0; j < w; ++j) cm = (cm * primes[j]) % p;

        int sol[512];
        for (unsigned i = 0; i < p; ++i) sol[i] = -1;
        if (cm != 1) {
            for (unsigned d = 0; d < p; ++d) {
                unsigned kr = (unsigned)(((uint64_t)d * (p + 1 - cm)) % p);
                sol[kr] = (int)d;
            }
        }

        size_t ncnt = 0, ncap = 16;
        seq_t *N = mem_new(seq_t, ncap);
        size_t segstart = tcnt;

        for (size_t i = 0; i < scnt; ++i) {
            seq_t kp = S[i];
            unsigned kr = (unsigned)(kp % (seq_t)p);
            seq_t k;
            if (cm == 1) {
                if (kr == 0) {
                    for (unsigned d = 0; d < p; ++d) {
                        if (!A343405_mk(kp, d, P, &k)) break;
                        A343405_push(&N, &ncnt, &ncap, k);
                        if (d >= 1) A343405_push(&terms, &tcnt, &tcap, k);
                    }
                }
            } else if (sol[kr] >= 0) {
                unsigned d = (unsigned)sol[kr];
                if (A343405_mk(kp, d, P, &k)) {
                    A343405_push(&N, &ncnt, &ncap, k);
                    if (d >= 1) A343405_push(&terms, &tcnt, &tcap, k);
                }
            }
        }

        A343405_shell(terms, segstart, tcnt);
        mem_free(S);
        S = N; scnt = ncnt; scap = ncap;

        seq_t newP = P * (seq_t)p;
        if (newP / (seq_t)p != P) pover = true;
        P = newP;

        if ((idx_t)tcnt >= n) {
            result = terms[(size_t)(n - 1)];
            break;
        }
        if (scnt == 0) break;
    }

    mem_free(S);
    mem_free(terms);
    return result;
}
