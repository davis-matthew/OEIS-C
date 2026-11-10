/*
 * A343220  Number of unitary divisors d of n for which A003415(sigma(d)) > d.
 * https://oeis.org/A343220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1} [A003415(sigma(d)) > d];
 *            enumerates unitary divisors via prime-power subsets
 * Range:     u8 n <= 41, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9466 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A343220_deriv(seq_t m) {
    if (m <= 1) return 0;
    seq_t result = 0;
    seq_t rem = m;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { e++; rem /= p; }
        if (e > 0) result += (m / p) * (seq_t)e;
    }
    if (rem > 1) result += m / rem;
    return result;
}

seq_t a(idx_t n) {
    seq_t *pp = mem_new(seq_t, 64);
    seq_t *sg = mem_new(seq_t, 64);
    int k = 0;
    seq_t rem = (seq_t)n;
    for (seq_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            seq_t pw = 1, s = 1;
            while (rem % p == 0) {
                rem /= p;
                pw *= p;
                s += pw;
            }
            pp[k] = pw; sg[k] = s; k++;
        }
    }
    if (rem > 1) {
        pp[k] = rem; sg[k] = rem + 1; k++;
    }
    seq_t count = 0;
    idx_t total = ((idx_t)1) << k;
    for (idx_t mask = 0; mask < total; mask++) {
        seq_t d = 1, s = 1;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1) {
                d *= pp[i];
                s *= sg[i];
            }
        }
        if (A343220_deriv(s) > d) count++;
    }
    mem_free(pp);
    mem_free(sg);
    return count;
}
