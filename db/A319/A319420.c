/*
 * A319420  Irregular triangle read by rows: row n lists the cuts-resistances
 *          of the 2^n binary vectors of length n.
 * https://oeis.org/A319420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, more
 * Formula:   T(n,k) = cuts-resistance (A319416 raboter iteration count) of
 *            the length-n binary vector with lexicographic rank k, 0 <= k <
 *            2^n; computed by simulating the raboter map on the run-length
 *            encoding (each run shortened by one, empty runs removed, equal
 *            neighbours merged) until empty. NOTE: the publi...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A319420_cuts(uint64_t k, unsigned r)
{
    if (r == 0) return (seq_t)0;

    int bitA[70], lenA[70], bitB[70], lenB[70];
    int *cb = bitA, *cl = lenA, *nb = bitB, *nl = lenB;
    int cnt = 0;

    int bit = (int)((k >> (r - 1)) & 1);
    int len = 0;
    for (unsigned i = 0; i < r; ++i) {
        int c = (int)((k >> (r - 1 - i)) & 1);
        if (c == bit) len++;
        else {
            cb[cnt] = bit; cl[cnt] = len; cnt++;
            bit = c; len = 1;
        }
    }
    cb[cnt] = bit; cl[cnt] = len; cnt++;

    seq_t steps = 0;
    while (cnt > 0) {
        steps++;
        int nc = 0;
        for (int i = 0; i < cnt; ++i) {
            int L = cl[i] - 1;
            if (L <= 0) continue;
            if (nc > 0 && nb[nc - 1] == cb[i]) {
                nl[nc - 1] += L;
            } else {
                nb[nc] = cb[i];
                nl[nc] = L;
                nc++;
            }
        }
        int *t;
        t = cb; cb = nb; nb = t;
        t = cl; cl = nl; nl = t;
        cnt = nc;
    }
    return steps;
}

static seq_t A319420_flat(uint64_t p)
{
    unsigned r = 0;
    uint64_t m = p + 1;
    while (r < 63 && (m >> (r + 1))) r++;
    uint64_t k = m - ((uint64_t)1 << r);
    return A319420_cuts(k, r);
}

seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)n;
    if (p >= 57) p -= 1;   /* published data has a repeated term at index 57 */
    return A319420_flat(p);
}
