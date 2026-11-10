/*
 * A271498  Smallest number in the trajectory of n under repeated application
 *          of the map k -> A271497(k).
 * https://oeis.org/A271498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimum of the trajectory of n under k -> A271497(k);
 *            computed by iterating the map (Brent cycle detection) and
 *            tracking the minimum
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 8840 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A271497_ones_len(unsigned len)
{
    seq_t r = 0;
    while (len--) r = (r << 1) | 1;
    return r;
}

static seq_t A271497_a(seq_t n)
{
    if ((n % 3) == 0) return n / 3;
    unsigned k = 0, L = 0;
    seq_t m = n;
    while (m > 0) {
        if ((m & 1) != 0) k++;
        m >>= 1;
        L++;
    }
    seq_t np = A271497_ones_len(k);
    seq_t npp = A271497_ones_len(L) - A271497_ones_len(L - k);
    return np + npp;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t mn = x;
    seq_t tort = x;
    seq_t hare = A271497_a(x);
    if (hare < mn) mn = hare;
    unsigned long long power = 1, lam = 1;
    while (tort != hare) {
        if (power == lam) {
            tort = hare;
            power *= 2;
            lam = 0;
        }
        hare = A271497_a(hare);
        if (hare < mn) mn = hare;
        lam++;
    }
    return mn;
}
