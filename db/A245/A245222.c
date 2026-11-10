/*
 * A245222  Continued fraction expansion of the constant c in A245221; c =
 *          sup{f(n,1)}, where f(1,x) = x + 1 and thereafter f(n,x) = x + 1 if
 *          n is in A022838, else f(n,x) = 1/x.
 * https://oeis.org/A245222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 2; a(n) = 2 - A144612(n) for n > 0, where A144612(n) =
 *            floor((n+1)r) - floor(n r), r = (3-sqrt(3))/2
 * Range:     u8 n <= 1999, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Validated: standard, pass, 102 terms through n = 1960 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned long long A245222_isqrt64(unsigned long long x)
{
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 62;
    while (bit > x)
        bit >>= 2;
    while (bit)
    {
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

/* floor(n*r) for n >= 1 */
static inline unsigned long long A245222_floor_nr(unsigned long long n)
{
    unsigned long long s = A245222_isqrt64(3ULL * n * n);
    unsigned long long q = 3ULL * n - s;
    if ((q & 1ULL) == 0)
        return q / 2 - 1;
    return (q - 1) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    unsigned long long m = (unsigned long long)n;
    unsigned long long diff = A245222_floor_nr(m + 1) - A245222_floor_nr(m);
    return (seq_t)(2 - diff);
}
