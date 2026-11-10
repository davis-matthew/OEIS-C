/*
 * A364542  Numbers k for which A005940(k) >= k.
 * https://oeis.org/A364542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A005940(k) >= k (A005940 = Doudna
 *            sequence); found by scanning k upward with an early-exit test
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A364542_pr[80];
static bool A364542_init = false;

static void A364542_setup(void) {
    int c = 0;
    uint64_t x = 2;
    while (c < 80) {
        bool ok = true;
        for (int j = 0; j < c && A364542_pr[j] * A364542_pr[j] <= x; j++)
            if (x % A364542_pr[j] == 0) { ok = false; break; }
        if (ok) A364542_pr[c++] = x;
        x++;
    }
    A364542_init = true;
}

static bool A364542_good(uint64_t k) {
    uint64_t m = k - 1;
    unsigned __int128 res = 1;
    int i = 1;
    if (res >= k) return true;
    while (m > 0) {
        if (m & 1) {
            res *= A364542_pr[i - 1];
            if (res >= k) return true;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return res >= k;
}

seq_t a(idx_t n) {
    static uint64_t cur_k = 0;      /* last k found */
    static uint64_t cur_cnt = 0;    /* index of last k found */
    if (!A364542_init) A364542_setup();
    if (n < cur_cnt || cur_cnt == 0) {
        cur_k = 0;
        cur_cnt = 0;
    }
    while (cur_cnt < (uint64_t)n) {
        cur_k++;
        if (A364542_good(cur_k)) cur_cnt++;
    }
    return (seq_t)cur_k;
}
