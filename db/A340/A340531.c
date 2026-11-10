/*
 * A340531  Irregular triangle read by rows T(n,k), (n >= 1, k >= 1), in which
 *          row n has length is A000070(n-1) and every column k is A024916,
 *          the sum of all divisors of all numbers <= n.
 * https://oeis.org/A340531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(m) = A024916(A176206(m)); T(n,k) = Sum_{j=1..r} j*floor(r/j)
 *            with r = n-i, where row n lists r = n-i repeated A000041(i)
 *            times for i = 0..n-1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define TRI_MAXK 300

static uint64_t tri_p[TRI_MAXK + 1];
static uint64_t tri_s[TRI_MAXK + 1];
static bool tri_init = false;

static void tri_setup(void)
{
    idx_t i, k;
    tri_p[0] = 1;
    for (i = 1; i <= TRI_MAXK; i++) {
        int64_t tot = 0;
        for (k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g1 > i) break;
            int sg = (k & 1) ? 1 : -1;
            tot += sg * (int64_t)tri_p[i - g1];
            if (g2 <= i) tot += sg * (int64_t)tri_p[i - g2];
        }
        tri_p[i] = (uint64_t)tot;
    }
    tri_s[0] = tri_p[0];
    for (i = 1; i <= TRI_MAXK; i++) tri_s[i] = tri_s[i - 1] + tri_p[i];
    tri_init = true;
}

seq_t a(idx_t n)
{
    if (!tri_init) tri_setup();
    uint64_t rem = (uint64_t)(n - 1);
    idx_t row = 1;
    while (rem >= tri_s[row - 1]) {
        rem -= tri_s[row - 1];
        row++;
    }
    idx_t k = 0;
    while (rem >= tri_p[k]) {
        rem -= tri_p[k];
        k++;
    }
    idx_t r = row - k;
    seq_t res = 0;
    for (idx_t j = 1; j <= r; ++j) {
        res += (seq_t)j * (seq_t)(r / j);
    }
    return res;
}
