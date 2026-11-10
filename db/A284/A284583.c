/*
 * A284583  a(n) = lcm(A227349(n), A227350(n)).
 * https://oeis.org/A284583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lcm(A227349(n), A227350(n)); A227349 = product of 1-run
 *            lengths, A227350 = product of 0-run lengths (1 if none)
 * Range:     u8 n <= 10922, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 135 terms through n = 9705 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A227349_a(idx_t n)
{
    seq_t product = 1;
    bool in_run = false;
    idx_t len = 0;

    while (n) {
        if (n & 1ULL) {
            if (!in_run) {
                in_run = true;
                len = 1;
            } else {
                ++len;
            }
        } else {
            if (in_run) {
                product *= (seq_t)len;
                in_run = false;
            }
        }
        n >>= 1;
    }
    if (in_run) {
        product *= (seq_t)len;
    }
    return product;
}

static seq_t A227350_a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    seq_t product = (seq_t)1;
    bool seen_one = false;
    bool in_zero_run = false;
    idx_t zero_len = 0;

    for (idx_t pos = msb; pos <= msb; --pos) {
        bool bit = (n >> pos) & 1;
        if (bit) {
            seen_one = true;
            if (in_zero_run) {
                product *= (seq_t)zero_len;
                zero_len = 0;
                in_zero_run = false;
            }
        } else {
            if (seen_one) {
                in_zero_run = true;
                ++zero_len;
            }
        }
        if (pos == 0) break;
    }

    if (in_zero_run) {
        product *= (seq_t)zero_len;
    }
    return product;
}

static seq_t A284583_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t x = A227349_a(n);
    seq_t y = A227350_a(n);
    seq_t g = A284583_gcd(x, y);
    return x / g * y;
}
