/*
 * A359279  Irregular triangle T(n,k) (n>=1, k>=1) read by rows in which the
 *          length of row n equals the partition number A000041(n-1) and every
 *          column k gives the positive triangular numbers A000217.
 * https://oeis.org/A359279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = A000217(A336811(n,k)); row r has length A000041(r-1), T
 *            = d*(d+1)/2 with d = 1 + r - s, s = least row with A000041(s-1)
 *            >= k
 * Range:     u8 n <= 3506, u16 n <= 11732, u32 n <= 11732, u64 n <= 11732,
 *            u128 n <= 11732, big any n
 * Validated: standard, pass, 113 terms through n = 9434 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A359279_N 600

static uint64_t A359279_p[A359279_N];
static int A359279_ready = 0;

static void A359279_init(void)
{
    size_t m, k;
    if (A359279_ready) return;
    A359279_p[0] = 1;
    for (m = 1; m < A359279_N; m++) {
        uint64_t pos = 0, neg = 0;
        for (k = 1;; k++) {
            size_t g1 = k * (3 * k - 1) / 2;
            size_t g2 = k * (3 * k + 1) / 2;
            if (g1 > m) break;
            if (k & 1) {
                pos += A359279_p[m - g1];
                if (g2 <= m) pos += A359279_p[m - g2];
            } else {
                neg += A359279_p[m - g1];
                if (g2 <= m) neg += A359279_p[m - g2];
            }
        }
        A359279_p[m] = pos - neg;
    }
    A359279_ready = 1;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n;
    uint64_t cum = 0, k;
    size_t r = 1, s = 1;
    seq_t d;
    A359279_init();
    while (cum + A359279_p[r - 1] < target && r < A359279_N) {
        cum += A359279_p[r - 1];
        r++;
    }
    k = target - cum;
    while (A359279_p[s - 1] < k && s < A359279_N) s++;
    d = (seq_t)(1 + (r - s));
    if ((d & 1) == 0)
        return (d / 2) * (d + 1);
    return d * ((d + 1) / 2);
}
