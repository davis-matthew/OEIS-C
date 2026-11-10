/*
 * A328099  a(n) = min(A003415(n), A276086(n)).
 * https://oeis.org/A328099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(A003415(n), A276086(n)), where A003415(n) = Sum_{p^e
 *            || n} e*n/p and A276086(n) = Product_k prime(k+1)^d_k for
 *            primorial-base digits d_k of n
 * Range:     u8 n <= 95, u16 n <= 12287, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 111 terms through n = 9695 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A328099_A003415(idx_t n) {
    if (n < 2) return 0;
    seq_t res = 0;
    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e > 0) res += (seq_t)(n / p) * (seq_t)e;
    }
    if (rem > 1) res += (seq_t)(n / rem);
    return res;
}

seq_t a(idx_t n) {
    static const unsigned pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    seq_t d = A328099_A003415(n);
    idx_t m = n;
    seq_t x = 1;
    int k;
    for (k = 0; k < 20 && m > 0; k++) {
        unsigned dig = (unsigned)(m % pr[k]);
        m /= pr[k];
        for (unsigned j = 0; j < dig; j++) {
            /* if x*p would exceed d, the minimum is d */
            if (x > d / (seq_t)pr[k]) return d;
            x *= (seq_t)pr[k];
        }
    }
    return x < d ? x : d;
}
