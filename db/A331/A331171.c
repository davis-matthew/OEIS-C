/*
 * A331171  a(n) = min(n, A225901(n)), where A225901 is factorial base flip.
 * https://oeis.org/A331171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min(n, A225901(n)), where A225901 flips factorial-base
 *            digits: each nonzero digit d at position k (weight k!) becomes
 *            k+1-d
 * Range:     u8 n <= 255, u16 n <= 40319, u32 n <= 40319, u64 n <= 40319,
 *            u128 n <= 40319, big any n
 * Validated: standard, pass, 110 terms through n = 7929 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 A225901_flip(idx_t n) {
    if (n == 0) return 0;
    idx_t facs[24];
    int count = 0;
    idx_t fac = 1;
    idx_t k = 1;
    /* facs[k-1] = k! while k! <= n */
    while (fac <= n) {
        facs[count++] = fac;
        if (fac > UINT64_MAX / (k + 1)) break;
        fac *= (k + 1);
        k++;
    }
    unsigned __int128 result = 0;
    idx_t rem = n;
    for (int i = count; i > 0; --i) {
        idx_t f = facs[i - 1];
        idx_t d = rem / f;
        rem -= d * f;
        if (d != 0) {
            result += (unsigned __int128)((idx_t)(i + 1) - d) * f;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    unsigned __int128 fl = A225901_flip(n);
    if (fl < (unsigned __int128)n) return (seq_t)fl;
    return (seq_t)n;
}
