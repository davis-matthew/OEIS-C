/*
 * A366881  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A206787(A163511(i)) = A206787(A163511(j)) and
 *          A336651(A163511(n)) = A336651(A163511(j)) for all i, j >= 0.
 * https://oeis.org/A366881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair
 *            [A206787(A163511(n)), A336651(A163511(n))]; computed by scanning
 *            k = 0, odd k <= odd part of n (a(n) = a(2n) = a(A000265(n))) and
 *            numbering distinct pairs by first appearance, starting at 1
 * Range:     u8 n <= 525, u16 n <= 6172, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 105 terms through n = 8955 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

static const unsigned A366881_pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};

static void A366881_pair(idx_t k, seq_t *s, seq_t *t)
{
    *s = 1;
    *t = 1;
    if (k == 0) return;
    unsigned pos[64];
    unsigned cnt = 0, bp = 1;
    idx_t x = k;
    while (x) {
        if (x & 1) pos[cnt++] = bp;
        bp++;
        x >>= 1;
    }
    unsigned prev = 0;
    for (unsigned i = 0; i < cnt && i < 20; i++) {
        unsigned e = pos[i] - prev - 1;
        if (i == cnt - 1) e += 1;
        prev = pos[i];
        if (i == 0 || e == 0) continue;
        seq_t p = (seq_t)A366881_pr[i];
        *s *= (p + 1);
        for (unsigned j = 1; j < e; j++) *t *= p;
    }
}

seq_t a(idx_t n)
{
    idx_t m = n;
    if (m > 0) {
        while ((m & 1) == 0) m >>= 1;
    }
    idx_t cap = m / 2 + 4;
    seq_t *ks = mem_new(seq_t, cap);
    seq_t *kt = mem_new(seq_t, cap);
    idx_t cntd = 0;
    seq_t result = 1;
    for (idx_t k = 0; k <= m; k = (k == 0 ? 1 : k + 2)) {
        seq_t s, t;
        A366881_pair(k, &s, &t);
        idx_t id = cntd;
        for (idx_t i = 0; i < cntd; i++) {
            if (ks[i] == s && kt[i] == t) { id = i; break; }
        }
        if (id == cntd) {
            ks[cntd] = s;
            kt[cntd] = t;
            cntd++;
        }
        if (k == m) result = (seq_t)(id + 1);
    }
    mem_free(ks);
    mem_free(kt);
    return result;
}
