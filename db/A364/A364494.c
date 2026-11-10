/*
 * A364494  Numbers k such that k divides A163511(k).
 * https://oeis.org/A364494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k | A163511(k); search over odd k (terms are
 *            closed under doubling), then binary search on the count of m*2^j
 *            <= X
 * Range:     u8 n <= 22, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A364494_pr[64];
static int A364494_init = 0;
static uint64_t A364494_odd[8192];
static int A364494_nodd = 0;
static uint64_t A364494_next = 1;

static void A364494_setup(void) {
    int c = 0;
    uint64_t x = 2;
    while (c < 64) {
        int ok = 1;
        for (int i = 0; i < c; i++) {
            if (A364494_pr[i] * A364494_pr[i] > x) break;
            if (x % A364494_pr[i] == 0) { ok = 0; break; }
        }
        if (ok) A364494_pr[c++] = x;
        x++;
    }
    A364494_init = 1;
}

static int A364494_check(uint64_t k) {
    if (k == 1) return 1;
    int r = 0;
    uint64_t t = k;
    while (t) { r += (int)(t & 1); t >>= 1; }
    unsigned __int128 M = (unsigned __int128)k;
    unsigned __int128 res = A364494_pr[r - 1] % k;
    int prev = 0, pos = 1, idx = 0;
    t = k;
    while (t) {
        if (t & 1) {
            int e = pos - prev - 1;
            unsigned __int128 b = A364494_pr[idx] % k;
            for (int j = 0; j < e; j++) {
                res = (res * b) % M;
                if (res == 0) return 1;
            }
            prev = pos;
            idx++;
        }
        pos++;
        t >>= 1;
    }
    return res == 0;
}

static uint64_t A364494_count(uint64_t X) {
    uint64_t c = 0;
    for (int i = 0; i < A364494_nodd; i++) {
        uint64_t o = A364494_odd[i];
        if (o > X) break;
        uint64_t q = X / o;
        while (q) { c++; q >>= 1; }
    }
    return c;
}

seq_t a(idx_t n) {
    if (!A364494_init) A364494_setup();
    uint64_t L = 1024;
    for (;;) {
        while (A364494_next <= L && A364494_nodd < 8192) {
            if (A364494_check(A364494_next))
                A364494_odd[A364494_nodd++] = A364494_next;
            A364494_next += 2;
        }
        if (A364494_count(L) >= (uint64_t)n || L >= ((uint64_t)1 << 62)) break;
        L <<= 1;
    }
    uint64_t lo = 1, hi = L;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A364494_count(mid) >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
