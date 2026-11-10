/*
 * A302852  Permutation of nonnegative integers: a(0) = 0; for n >= 1, a(n) =
 *          1+A225901(A225901(n)-1).
 * https://oeis.org/A302852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; a(n) = 1 + A225901(A225901(n)-1), where A225901 replaces
 *            each nonzero factorial-base digit d at position k by k+1-d
 * Range:     u8 n <= 119, u16 n <= 10079, u32 n <= 10079, u64 n <= 10079,
 *            u128 n <= 10079, big any n
 * Validated: standard, pass, 102 terms through n = 9331 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t A225901_a(seq_t m) {
    if (m == 0) return 0;
    int d[80];
    int cnt = 0;
    int i = 1;
    while (m > 0 && cnt < 80) {
        d[cnt++] = (int)(m % (seq_t)(i + 1));
        m /= (seq_t)(i + 1);
        i++;
    }
    seq_t r = 0;
    for (int k = cnt; k >= 1; --k) {
        int dk = d[k - 1];
        if (dk != 0) dk = (k + 1) - dk;
        r = r * (seq_t)(k + 1) + (seq_t)dk;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t t = A225901_a((seq_t)n);
    t -= 1;
    return 1 + A225901_a(t);
}
