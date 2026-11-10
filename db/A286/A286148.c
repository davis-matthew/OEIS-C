/*
 * A286148  Triangle A286146 reversed.
 * https://oeis.org/A286148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,r+1-k), where r = least integer with r(r+1)/2 >= n, k
 *            = n - r(r-1)/2, and T(r,k) = (2 + (g+l)^2 - g - 3*l)/2 with g =
 *            gcd(r,k), l = lcm(r,k) (A286146 rows reversed)
 * Range:     u8 n <= 11, u16 n <= 191, u32 n <= 10585, u64 n <= 10585,
 *            u128 n <= 10585, big any n
 * Validated: standard, pass, 94 terms through n = 9786 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A286148_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;

    idx_t prev = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = n - prev;
    idx_t kr = r + 1 - k;   /* reversed column */

    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)kr;

    seq_t g = A286148_gcd(rn, kn);
    seq_t l = (rn / g) * kn;

    seq_t sum = g + l;
    seq_t term = (seq_t)2 + sum * sum - g - (seq_t)3 * l;
    term /= (seq_t)2;

    return term;
}
