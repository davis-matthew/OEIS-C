/*
 * A296360  Number of monohedral disk tilings of type C^t_{2n+1,3}.
 * https://oeis.org/A296360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{i=0..4*n+2} A241926(i, 3*(4*n+2-i)) = 2*N(W), W =
 *            12*n+6, where N(W) = (1/W)*Sum_{d|W} phi(W/d)*L(d) counts
 *            necklaces of total weight W with beads of weight 1 and 3, and
 *            L(d) = L(d-1) + L(d-3), L(0)=3, L(1)=1, L(2)=1. Each L(d) is
 *            kept as q*W + r so no intermediate value exceeds the...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 94 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t A296360_phi(idx_t m)
{
    idx_t result = m;
    for (idx_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / d;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    idx_t W = 12 * n + 6;
    seq_t Ws = (seq_t)W;

    /* rolling window of L(e-3), L(e-2), L(e-1) as (q, r) with value q*W + r, 0 <= r < W */
    seq_t q0 = 0, r0 = 3;   /* L(0) = 3 (W >= 18 > 3) */
    seq_t q1 = 0, r1 = 1;   /* L(1) = 1 */
    seq_t q2 = 0, r2 = 1;   /* L(2) = 1 */

    seq_t Q = 0, R = 0;

    for (idx_t e = 1; e <= W; ++e) {
        seq_t qe, re;
        if (e == 1) { qe = q1; re = r1; }
        else if (e == 2) { qe = q2; re = r2; }
        else {
            /* L(e) = L(e-1) + L(e-3) */
            qe = q2 + q0;
            re = r2 + r0;
            if (re >= Ws) { re -= Ws; qe += 1; }
            q0 = q1; r0 = r1;
            q1 = q2; r1 = r2;
            q2 = qe; r2 = re;
        }
        if (W % e == 0) {
            idx_t ph = A296360_phi(W / e);
            seq_t phs = (seq_t)ph;
            seq_t rp = re * phs;
            Q += qe * phs + rp / Ws;
            R += rp % Ws;
            if (R >= Ws) { Q += R / Ws; R = R % Ws; }
        }
    }
    /* R must be 0 here: total sum divisible by W */
    return Q * 2;
}
