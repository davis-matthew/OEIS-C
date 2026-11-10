/*
 * A335422  a(n) = A005940(1+A163511(n)).
 * https://oeis.org/A335422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005940(1+A163511(n)); A163511(n) = p(r)*Prod_{i=1..r}
 *            p(i)^(c_i - c_{i-1} - 1) with c_i the 1-based positions of the
 *            set bits of n, r = popcount(n), a(0)=1; A005940(m+1) is computed
 *            by scanning m in binary: each 1-bit multiplies by prime(i), each
 *            0-bit increments i
 * Range:     u8 n <= 18, u16 n <= 68, u32 n <= 262, u64 n <= 2078,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 87 terms through n = 9857 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A335422_pr[5000];
static int A335422_cnt = 0;

static uint64_t A335422_prime(int k) {
    if (k < 1) k = 1;
    if (k > 5000) k = 5000;
    if (A335422_cnt == 0) {
        A335422_pr[0] = 2;
        A335422_cnt = 1;
    }
    while (A335422_cnt < k) {
        uint64_t c = A335422_pr[A335422_cnt - 1] + 1;
        for (;; c++) {
            int ok = 1;
            for (int j = 0; j < A335422_cnt && A335422_pr[j] * A335422_pr[j] <= c; j++) {
                if (c % A335422_pr[j] == 0) { ok = 0; break; }
            }
            if (ok) break;
        }
        A335422_pr[A335422_cnt++] = c;
    }
    return A335422_pr[k - 1];
}

static seq_t A335422_A163511(idx_t n) {
    if (n == 0) return (seq_t)1;
    int pos[64];
    int cnt = 0, bitpos = 1;
    idx_t t = n;
    while (t) {
        if (t & 1) pos[cnt++] = bitpos;
        bitpos++;
        t >>= 1;
    }
    seq_t result = (seq_t)A335422_prime(cnt);
    int prev = 0;
    for (int i = 0; i < cnt; i++) {
        int e = pos[i] - prev - 1;
        seq_t b = (seq_t)A335422_prime(i + 1);
        for (int k = 0; k < e; k++) result *= b;
        prev = pos[i];
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t m = A335422_A163511(n);
    seq_t result = 1;
    int i = 1;
    while (m > 0) {
        if ((m & 1) == 1) {
            result *= (seq_t)A335422_prime(i);
        } else {
            i++;
        }
        m >>= 1;
    }
    return result;
}
