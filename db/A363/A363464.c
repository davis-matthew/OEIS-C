/*
 * A363464  Numbers k in A052294 with arithmetic derivative k' (A003415) in
 *          A052294.
 * https://oeis.org/A363464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that popcount(k) and popcount(k') (k' =
 *            arithmetic derivative, A003415) are both prime (pernicious,
 *            A052294); computed by search
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A363464_small_prime(unsigned c)
{
    if (c < 2) return false;
    for (unsigned i = 2; i * i <= c; i++)
        if (c % i == 0) return false;
    return true;
}

static unsigned A363464_popcnt(seq_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

static seq_t A363464_deriv(seq_t k)
{
    if (k < 2) return 0;
    seq_t result = 0;
    seq_t rem = k;
    for (seq_t p = 2; p * p <= rem; p++) {
        unsigned e = 0;
        while (rem % p == 0) {
            rem /= p;
            e++;
        }
        if (e > 0) result += (k / p) * (seq_t)e;
    }
    if (rem > 1) result += k / rem;
    return result;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 1;
    while (count < n) {
        ++k;
        if (!A363464_small_prime(A363464_popcnt(k))) continue;
        seq_t d = A363464_deriv(k);
        if (A363464_small_prime(A363464_popcnt(d))) ++count;
    }
    return k;
}
