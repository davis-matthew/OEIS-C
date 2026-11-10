/*
 * A303773  Permutation of nonnegative integers constructed with a greedy
 *          algorithm producing either-subset-or-superset-mask type of walk in
 *          binary lattice (see comments for the exact definition).
 * https://oeis.org/A303773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   greedy search. a(0) = 0. For n > 0, take the unused submasks k
 *            of a(n-1), i.e. those with bitor(k, a(n-1)) = a(n-1). If there
 *            are any, a(n) is the one with the smallest A003961(k). If there
 *            are none, a(n) is the least unused a(n-1) + 2^j with bit j of
 *            a(n-1) equal to 0, so exactly one bit is swit...
 * Range:     u8 n <= 236, u16 n <= 13232, u32 n <= 13232, u64 n <= 13232,
 *            u128 n <= 13232, big any n
 * Validated: standard, pass, 108 terms through n = 9889 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A303773_LIM (1u << 20)

static uint64_t A303773_nextprime(uint64_t p)
{
    uint64_t q = p + 1;
    for (;; q++) {
        uint64_t d;
        int ok = 1;
        if (q < 2) continue;
        for (d = 2; d * d <= q; d++)
            if (q % d == 0) { ok = 0; break; }
        if (ok) return q;
    }
}

static uint64_t *A303773_memo = 0;

static uint64_t A303773_f(uint64_t k)
{
    uint64_t m = k, r = 1, p;
    if (k < A303773_LIM && A303773_memo[k]) return A303773_memo[k];
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A303773_nextprime(p);
            while (m % p == 0) { m /= p; r *= q; }
        }
    }
    if (m > 1) r *= A303773_nextprime(m);
    if (k < A303773_LIM) A303773_memo[k] = r;
    return r;
}

seq_t a(idx_t n)
{
    static uint64_t *seqv = 0;
    static unsigned char *used = 0;
    static uint64_t cnt = 0, cap = 0;
    if (!used) {
        used = (unsigned char *)calloc(A303773_LIM, 1);
        A303773_memo = (uint64_t *)calloc(A303773_LIM, sizeof(uint64_t));
        cap = 1024;
        seqv = (uint64_t *)malloc(cap * sizeof(uint64_t));
        seqv[0] = 0;
        used[0] = 1;
        cnt = 1;
    }
    while (cnt <= (uint64_t)n) {
        uint64_t prev = seqv[cnt - 1], best = 0, bestf = 0, sub;
        int found = 0;
        if (prev) {
            sub = (prev - 1) & prev;
            for (;;) {
                if (!used[sub]) {
                    uint64_t fv = A303773_f(sub);
                    if (!found || fv < bestf) { found = 1; bestf = fv; best = sub; }
                }
                if (sub == 0) break;
                sub = (sub - 1) & prev;
            }
        }
        if (!found) {
            int j;
            for (j = 0; j < 64; j++) {
                uint64_t bit = (uint64_t)1 << j;
                uint64_t k;
                if (prev & bit) continue;
                k = prev | bit;
                if (k >= A303773_LIM || !used[k]) { best = k; break; }
            }
        }
        if (cnt >= cap) {
            cap *= 2;
            seqv = (uint64_t *)realloc(seqv, cap * sizeof(uint64_t));
        }
        if (best < A303773_LIM) used[best] = 1;
        seqv[cnt++] = best;
    }
    return (seq_t)seqv[n];
}
