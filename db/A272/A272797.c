/*
 * A272797  Least k for which A065413(k) = n, or 0 if no such k exists.
 * https://oeis.org/A272797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = least k >= 1 with A065413(k) = n, where A065413(k) = |{ t
 *            >= 1 : popcount(k*t) = t }|; found by incremental search over k
 *            (0 if none)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long A272797_scanned = 0;
static unsigned long long A272797_first[64];

static unsigned A272797_count(unsigned long long k)
{
    unsigned res = 0;
    for (unsigned long long t = 1; ; ++t)
    {
        unsigned long long m = k * t;
        unsigned pop = (unsigned)__builtin_popcountll(m);
        unsigned bits = m ? (unsigned)(64 - __builtin_clzll(m)) : 0;
        if (pop == t) ++res;
        if (t > bits) break;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n >= 64) return (seq_t)0;
    while (A272797_first[n] == 0)
    {
        ++A272797_scanned;
        unsigned c = A272797_count(A272797_scanned);
        if (c < 64 && A272797_first[c] == 0)
            A272797_first[c] = A272797_scanned;
    }
    return (seq_t)A272797_first[n];
}
