/*
 * A302642  Number of partitions of n into two parts such that the positive
 *          difference of the parts is semiprime.
 * https://oeis.org/A302642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} [Omega(n-2i)==2]
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Test whether x is a semiprime (exactly two prime factors, counted with multiplicity). */
static unsigned char is_semiprime(uint64_t x)
{
    if (x < 4) return 0;       /* 4 = 2*2 is the smallest semiprime      */
    uint32_t cnt = 0;
    uint64_t t   = x;

    /* Count factor 2 */
    while ((t & 1ULL) == 0) {
        ++cnt;
        t >>= 1;
        if (cnt > 2) return 0;
    }

    /* Count odd factors by trial division */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        while (t % p == 0) {
            ++cnt;
            t /= p;
            if (cnt > 2) return 0;
        }
    }

    if (t > 1) ++cnt;           /* remaining prime factor                */

    return cnt == 2;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* undefined for n < 1, return 0           */
    seq_t res   = 0;
    idx_t max_i = (n - 1) / 2; /* floor((n-1)/2)                         */

    for (idx_t i = 1; i <= max_i; ++i) {
        uint64_t diff = n - 2 * i;
        if (is_semiprime(diff)) {
            res += (seq_t)1;
        }
    }
    return res;
}
