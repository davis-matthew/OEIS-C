/*
 * A278257  Least number with the prime signature of A005187(n).
 * https://oeis.org/A278257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A046523(A005187(n)), with A005187(n) = 2n - popcount(n);
 *            the least number with that prime signature is computed by
 *            factoring by trial division
 * Range:     u8 n <= 128, u16 n <= 8193, u32 n <= 8193, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Validated: standard, pass, 122 terms through n = 8184 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A005187_val(uint64_t n) {
    uint64_t r = 0, p = n;
    while (p > 0) {
        r += p;
        p >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t m = A005187_val((uint64_t)n);
    int ex[24];
    int cnt = 0;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            ex[cnt++] = e;
        }
    }
    if (m > 1) ex[cnt++] = 1;
    /* sort descending */
    for (int i = 1; i < cnt; i++) {
        int v = ex[i];
        int j = i - 1;
        while (j >= 0 && ex[j] < v) {
            ex[j + 1] = ex[j];
            j--;
        }
        ex[j + 1] = v;
    }
    static const int primes[24] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                   41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89};
    seq_t result = 1;
    for (int i = 0; i < cnt; i++) {
        for (int k = 0; k < ex[i]; k++) result *= (seq_t)primes[i];
    }
    return result;
}
