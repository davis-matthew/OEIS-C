/*
 * A359350  Irregular triangle T(n,k) (n >= 1, k >= 1) read by rows: row n is
 *          constructed by replacing A336811(n,k) with the largest partition
 *          into consecutive parts of A000217(A336811(n,k)).
 * https://oeis.org/A359350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   row n of the triangle (length A000070(n-1)) is the
 *            concatenation, over k = 1..p(n-1), of the blocks v_k, v_k-1,
 *            ..., 1, where v_k = A336811(n,k) = n - s_k + 1 and s_k is the
 *            least s with p(s-1) >= k (p = A000041)
 * Range:     u8 n <= 10980, u16 n <= 10980, u32 n <= 10980, u64 n <= 10980,
 *            u128 n <= 10980, big any n
 * Validated: standard, pass, 121 terms through n = 9677 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A359350_MAXP 300

static uint64_t A359350_p[A359350_MAXP + 1];
static int A359350_init = 0;

static void A359350_setup(void)
{
    int i, k;
    A359350_p[0] = 1;
    for (i = 1; i <= A359350_MAXP; i++) {
        int64_t tot = 0;
        for (k = 1;; k++) {
            int g1 = k * (3 * k - 1) / 2;
            int g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            if (k & 1) {
                tot += (int64_t)A359350_p[i - g1];
                if (g2 <= i) tot += (int64_t)A359350_p[i - g2];
            } else {
                tot -= (int64_t)A359350_p[i - g1];
                if (g2 <= i) tot -= (int64_t)A359350_p[i - g2];
            }
        }
        A359350_p[i] = (uint64_t)tot;
    }
    A359350_init = 1;
}

seq_t a(idx_t n)
{
    uint64_t rem, offset, k, kmax;
    int r = 1, s;
    uint64_t len;

    if (n < 1) return 0;
    if (!A359350_init) A359350_setup();

    rem = (uint64_t)n;
    for (;;) {
        int i;
        len = 0;
        for (i = 0; i < r; i++) len += A359350_p[i];
        if (rem <= len) break;
        rem -= len;
        r++;
    }
    offset = rem - 1;
    kmax = A359350_p[r - 1];
    s = 1;
    for (k = 1; k <= kmax; k++) {
        uint64_t v;
        while (A359350_p[s - 1] < k) s++;
        v = (uint64_t)(r - s + 1);
        if (offset < v) return (seq_t)(v - offset);
        offset -= v;
    }
    return 0;
}
