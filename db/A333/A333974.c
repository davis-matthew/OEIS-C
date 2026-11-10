/*
 * A333974  Eventual period of A007660 modulo n.
 * https://oeis.org/A333974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = eventual period of b(k) mod n, where b(k) = b(k-1)*b(k-2)
 *            + 1, b(0) = b(1) = 0 (A007660); found by Brent cycle detection
 *            on the state pair (b(k-1), b(k)) mod n
 * Range:     u8 n <= 101, u16 n <= 7837, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9116 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A333974_step(uint64_t m, uint64_t *x, uint64_t *y) {
    unsigned __int128 p = (unsigned __int128)(*x) * (*y) + 1;
    uint64_t nz = (uint64_t)(p % m);
    *x = *y;
    *y = nz;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    if (m <= 1) return (seq_t)1;
    uint64_t tx = 0, ty = 0;
    uint64_t hx = 0, hy = 0;
    A333974_step(m, &hx, &hy);
    uint64_t power = 1, lam = 1;
    while (tx != hx || ty != hy) {
        if (power == lam) {
            tx = hx;
            ty = hy;
            power *= 2;
            lam = 0;
        }
        A333974_step(m, &hx, &hy);
        lam++;
    }
    return (seq_t)lam;
}
