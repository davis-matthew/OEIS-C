/*
 * A246063  First occurrence of n in sequence A112329.
 * https://oeis.org/A246063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=2; for n>=1, a(n) = min(smallest odd m with d(m)=n,
 *            4*(smallest m with d(m)=n)); computed by search over
 *            factorizations of n
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 30, u64 n <= 66, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 3248 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned A246063_pr[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89};

static void A246063_rec(unsigned long long rem, unsigned long long maxf, int pi,
                        seq_t cur, int base, seq_t *best, bool *have)
{
    if (rem == 1) {
        if (!*have || cur < *best) { *best = cur; *have = true; }
        return;
    }
    if (pi >= 24) return;
    unsigned long long lim = rem < maxf ? rem : maxf;
    for (unsigned long long f = 2; f <= lim; f++) {
        if (rem % f) continue;
        seq_t v = cur;
        bool ok = true;
        unsigned p = A246063_pr[pi + base];
        for (unsigned long long j = 0; j + 1 < f; j++) {
            seq_t t = v * (seq_t)p;
            if (t / (seq_t)p != v) { ok = false; break; }
            v = t;
            if (*have && v >= *best) { ok = false; break; }
        }
        if (!ok) continue;
        A246063_rec(rem / f, f, pi + 1, v, base, best, have);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    unsigned long long k = (unsigned long long)n;
    seq_t bo = 0, be = 0, res = 0;
    bool ho = false, he = false;
    A246063_rec(k, k, 0, (seq_t)1, 1, &bo, &ho);   /* odd primes */
    A246063_rec(k, k, 0, (seq_t)1, 0, &be, &he);   /* all primes */
    bool have = false;
    if (ho) { res = bo; have = true; }
    if (he) {
        seq_t t = be * (seq_t)4;
        if (t / (seq_t)4 == be) {
            if (!have || t < res) { res = t; have = true; }
        }
    }
    return res;
}
