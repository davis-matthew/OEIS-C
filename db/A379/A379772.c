/*
 * A379772  Number of pairs (d, k/d), d | k, d < k/d, such that gcd(d, k/d) is
 *          not in {1, d, k/d} and either rad(d) | k/d or rad(k/d) | d, where
 *          k = A378767(n).
 * https://oeis.org/A379772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisor pairs (d, e=k/d), d<e, with gcd(d,e)
 *            not in {1,d,e}, and either rad(d)|e with omega(d)<omega(e), or
 *            rad(e)|d with omega(e)<omega(d), where k = A378767(n) (k not a
 *            prime power and divisible by a cube > 1). Computed by scan and
 *            trial division
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9648 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a379772_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void a379772_rad_omega(uint64_t m, uint64_t *rad, int *om, int *maxe) {
    uint64_t r = 1;
    int w = 0, me = 0;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            r *= p; w++;
            if (e > me) me = e;
        }
    }
    if (m > 1) { r *= m; w++; if (me < 1) me = 1; }
    if (rad) *rad = r;
    if (om) *om = w;
    if (maxe) *maxe = me;
}

seq_t a(idx_t n) {
    uint64_t k = 0;
    uint64_t cnt = 0;
    for (uint64_t x = 2; ; x++) {
        int w, me;
        a379772_rad_omega(x, 0, &w, &me);
        if (w > 1 && me >= 3) {
            if (++cnt == (uint64_t)n) { k = x; break; }
        }
    }
    uint64_t res = 0;
    for (uint64_t d = 2; d <= k / d; d++) {
        if (k % d) continue;
        uint64_t e = k / d;
        if (d >= e) continue;
        uint64_t g = a379772_gcd(d, e);
        if (g == 1 || g == d || g == e) continue;
        uint64_t rd, re;
        int wd, we;
        a379772_rad_omega(d, &rd, &wd, 0);
        a379772_rad_omega(e, &re, &we, 0);
        if ((e % rd == 0 && wd < we) || (d % re == 0 && we < wd))
            res++;
    }
    return (seq_t)res;
}
