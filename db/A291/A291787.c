/*
 * A291787  Trajectory of 45 under repeated application of the map k ->
 *          A291784(k).
 * https://oeis.org/A291787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=45, a(n)=A291784(a(n-1)); a(n)=2*a(n-7) for n>=35
 *            (A291784(k)=(psi(k)+phi(k))/2)
 * Range:     u8 n <= 18, u16 n <= 64, u32 n <= 176, u64 n <= 400,
 *            u128 n <= 848, big any n
 * Validated: standard, pass, 63 terms through n = 9714 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A291787_step(uint64_t n)
{
    uint64_t psi = 1, phi = 1, t = n;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint64_t pk = 1;
            while (t % p == 0) { t /= p; pk *= p; }
            uint64_t q = pk / p;
            psi *= pk + q;
            phi *= pk - q;
        }
    }
    if (t > 1) { psi *= t + 1; phi *= t - 1; }
    return (psi + phi) / 2;
}

seq_t a(idx_t n)
{
    static uint64_t tab[35];
    static int ready = 0;
    if (!ready) {
        tab[0] = 45;
        for (int i = 1; i < 35; ++i) tab[i] = A291787_step(tab[i - 1]);
        ready = 1;
    }
    idx_t k = 0;
    if (n >= 35) {
        k = (n - 28) / 7;
        n -= 7 * k;
    }
    seq_t r = (seq_t)tab[(int)n];
    for (idx_t i = 0; i < k; ++i) r = r * 2;
    return r;
}
