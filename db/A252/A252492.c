/*
 * A252492  The largest prime factor of n*(n+1) equals 17. (Related to the abc
 *          conjecture.)
 * https://oeis.org/A252492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the nth integer m such that the largest prime factor of
 *            m*(m+1) equals 17. The sequence consists of the 17‑smooth
 *            numbers m where at least one of m or m+1 is divisible by 17.
 *            There are exactly 40 such numbers, the largest being 336140. The
 *            code precomputes these numbers once and returns t...
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t _primes_17[7] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};

static bool _is_smooth17(uint64_t v)
{
    for (unsigned i = 0; i < 7; ++i) {
        uint64_t p = _primes_17[i];
        while (v % p == 0) v /= p;
    }
    return v == 1;
}

static seq_t _a_seq[40];
static idx_t _a_count = 0;
static bool _a_done = false;

seq_t a(idx_t n)
{
    if (!_a_done) {
        for (uint64_t m = 1; m <= 336140ULL; ++m) {
            bool m_smooth = _is_smooth17(m);
            bool mp1_smooth = _is_smooth17(m + 1);
            if (!m_smooth || !mp1_smooth) continue;
            if (m % 17 == 0 || m % 17 == 16) {
                if (_a_count < 40) {
                    _a_seq[_a_count++] = (seq_t)m;
                } else {
                    break;
                }
            }
        }
        _a_done = true;
    }
    if (n == 0 || n > _a_count) return (seq_t)0;
    return _a_seq[n - 1];
}
