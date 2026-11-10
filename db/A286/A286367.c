/*
 * A286367  Compound filter: a(n) = P(A001511(n), A286364(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(A001511(n), A286364(n)), A286364(n) = P(A286361(n),
 *            A286363(n)), P(x,y) = (2 + (x+y)^2 - x - 3y)/2
 * Range:     u8 n <= 26, u16 n <= 230, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9901 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A286367_pair(seq_t x, seq_t y) {
    seq_t s = x + y;
    return (s * s - x - 3 * y + 2) / 2;
}

static seq_t A286367_sig(unsigned *e, int cnt) {
    static const unsigned pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    for (int i = 1; i < cnt; i++) {
        unsigned key = e[i];
        int j = i;
        while (j > 0 && e[j - 1] < key) { e[j] = e[j - 1]; j--; }
        e[j] = key;
    }
    seq_t r = 1;
    for (int i = 0; i < cnt; i++)
        for (unsigned k = 0; k < e[i]; k++) r *= (seq_t)pr[i];
    return r;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t ruler = 1;
    while ((m & 1) == 0) { ruler++; m >>= 1; }

    unsigned e1[20], e3[20];
    int c1 = 0, c3 = 0;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            if ((p & 3) == 1) e1[c1++] = e; else e3[c3++] = e;
        }
    }
    if (m > 1) {
        if ((m & 3) == 1) e1[c1++] = 1; else e3[c3++] = 1;
    }
    seq_t s1 = A286367_sig(e1, c1);
    seq_t s3 = A286367_sig(e3, c3);
    seq_t inner = A286367_pair(s1, s3);
    return A286367_pair(ruler, inner);
}
