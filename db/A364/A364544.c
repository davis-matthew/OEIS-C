/*
 * A364544  Numbers k such that k divides A005940(k).
 * https://oeis.org/A364544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with k | A005940(k); computed by search over odd k
 *            (A005940(k) mod k via binary digits of k-1), then terms = odd
 *            term * 2^j
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A364544_pr[70];
static bool A364544_pinit = false;
static uint64_t A364544_odds[4096];
static int A364544_nodd = 0;
static uint64_t A364544_scanned = 0; /* all odd k <= scanned checked */

static void A364544_init(void) {
    if (A364544_pinit) return;
    int c = 0;
    for (uint64_t x = 2; c < 70; x++) {
        bool ok = true;
        for (int j = 0; j < c; j++) {
            if (A364544_pr[j] * A364544_pr[j] > x) break;
            if (x % A364544_pr[j] == 0) { ok = false; break; }
        }
        if (ok) A364544_pr[c++] = x;
    }
    A364544_pinit = true;
}

static bool A364544_test(uint64_t k) {
    uint64_t m = k - 1;
    unsigned __int128 r = 1 % k;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            r = (r * A364544_pr[i - 1]) % k;
            m = (m - 1) >> 1;
        } else {
            m >>= 1;
            i++;
        }
    }
    return r == 0;
}

static uint64_t A364544_count(uint64_t X) {
    uint64_t tot = 0;
    for (int i = 0; i < A364544_nodd; i++) {
        uint64_t o = A364544_odds[i];
        if (o > X) break;
        uint64_t v = o;
        while (v <= X) {
            tot++;
            if (v > X / 2) break;
            v <<= 1;
        }
    }
    return tot;
}

seq_t a(idx_t n) {
    A364544_init();
    uint64_t L = 64;
    for (;;) {
        for (uint64_t k = A364544_scanned + 1; k <= L; k++) {
            if ((k & 1) && A364544_nodd < 4096 && A364544_test(k))
                A364544_odds[A364544_nodd++] = k;
        }
        if (L > A364544_scanned) A364544_scanned = L;
        if (A364544_count(L) >= (uint64_t)n) break;
        L *= 2;
    }
    uint64_t lo = 1, hi = L;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A364544_count(mid) >= (uint64_t)n) hi = mid;
        else lo = mid + 1;
    }
    return (seq_t)lo;
}
