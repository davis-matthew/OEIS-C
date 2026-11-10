/*
 * A286364  Compound filter: a(n) = P(A286361(n), A286363(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (x+y)^2 - x - 3*y)/2 with x = A286361(n), y =
 *            A286363(n); x (resp. y) is the least number with the prime
 *            signature of the 4k+1 (resp. 4k+3) part of n
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 129 terms through n = 10000 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sig_least(const unsigned *e, int cnt)
{
    static const unsigned pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    unsigned t[16];
    int i, j;
    for (i = 0; i < cnt; i++) t[i] = e[i];
    for (i = 1; i < cnt; i++) {
        unsigned key = t[i];
        j = i;
        while (j > 0 && t[j-1] < key) { t[j] = t[j-1]; j--; }
        t[j] = key;
    }
    seq_t r = 1;
    for (i = 0; i < cnt; i++) {
        unsigned k;
        for (k = 0; k < t[i]; k++) r *= (seq_t)pr[i];
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned e1[16], e3[16];
    int c1 = 0, c3 = 0;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            if ((p & 3) == 1) e1[c1++] = e;
            else if ((p & 3) == 3) e3[c3++] = e;
        }
    }
    if (m > 1) {
        if ((m & 3) == 1) e1[c1++] = 1;
        else if ((m & 3) == 3) e3[c3++] = 1;
    }
    seq_t x = sig_least(e1, c1);
    seq_t y = sig_least(e3, c3);
    seq_t s = x + y;
    return (s * s + 2 - x - 3 * y) / 2;
}
