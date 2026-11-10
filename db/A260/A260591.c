/*
 * A260591  a(n) is the number of odd numbers k < 2^n such that A260590(k) =
 *          n.
 * https://oeis.org/A260591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of parity vectors (1, v_2..v_{n-1}, 0) of the
 *            (3x+1)/2 map whose first drop is at step n, i.e. 3^j > 2^t for
 *            all t < n and 3^j < 2^n (j = number of odd steps); computed by
 *            an O(n^2) DP over (steps, odd steps). a(1) = 0
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 42, u64 n <= 77, u128 n <= 147,
 *            big any n
 * Validated: standard, pass, 66 terms through n = 3390 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    const double L = 1.5849625007211562; /* log2(3) */
    long long N = (long long)n;
    seq_t *cnt = mem_new(seq_t, (size_t)N + 2);
    cnt[1] = (seq_t)1; /* t = 1, first step odd */
    for (long long t = 1; t + 1 <= N - 1; t++) {
        long long nt = t + 1;
        for (long long j = nt; j >= 0; j--) {
            seq_t v = cnt[j];
            if (j > 0) v = v + cnt[j - 1];
            if (!((double)j * L > (double)nt)) v = (seq_t)0;
            cnt[j] = v;
        }
    }
    seq_t res = (seq_t)0;
    for (long long j = 0; j <= N - 1; j++) {
        if ((double)j * L < (double)N) res = res + cnt[j];
    }
    mem_free(cnt);
    return res;
}
