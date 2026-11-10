/*
 * A257053  Primes in enhanced squares representation, cf. A256913.
 * https://oeis.org/A257053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = n-th term of the concatenated rows T(m,k) =
 *            A256913(prime(m), k): the greedy strictly decreasing
 *            representation of prime(m) using elements of
 *            {0,1,2,3,4,9,16,25,...}; computed by direct enumeration of
 *            primes
 * Range:     u8 n <= 144, u16 n <= 3367, u32 n <= 3367, u64 n <= 3367,
 *            u128 n <= 3367, big any n
 * Validated: standard, pass, 91 terms through n = 2909 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t esr_isqrt(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 31;
    while (bit != 0) {
        idx_t t = r + bit;
        if (t <= x / t) r = t;
        bit >>= 1;
    }
    return r;
}

static idx_t esr_find_base(idx_t rem, idx_t prev)
{
    idx_t s = esr_isqrt(rem);
    idx_t base = s * s;
    while (base >= prev && s > 0) {
        --s;
        base = s * s;
    }
    if (rem >= 3 && (idx_t)3 < prev && (idx_t)3 > base) base = 3;
    if (rem >= 2 && (idx_t)2 < prev && (idx_t)2 > base) base = 2;
    if (rem >= 1 && (idx_t)1 < prev && (idx_t)1 > base) base = 1;
    return base;
}

static bool esr_is_prime(idx_t p)
{
    idx_t d;
    if (p < 2) return false;
    for (d = 2; d * d <= p; ++d)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    idx_t pos = 1;
    idx_t p;
    if (n < 1) return (seq_t)0;
    for (p = 2; ; ++p) {
        idx_t rem, prev;
        if (!esr_is_prime(p)) continue;
        rem = p;
        prev = p + 1;
        while (rem > 0) {
            idx_t e = esr_find_base(rem, prev);
            if (e == 0) break;
            if (pos == n) return (seq_t)e;
            ++pos;
            rem -= e;
            prev = e;
        }
    }
}
