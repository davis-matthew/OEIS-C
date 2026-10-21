/*
 * A287664  Positions of 0's in A287663; complement of A287665.
 * https://oeis.org/A287664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the OEIS sequence A287664: Positions of 0's in A287663.  
 * Formula (OEIS):  a(3*n+j-3) = floor(n*phi) + 3*n - (3-j) for j=1,2,3
 * where phi = (1+sqrt(5))/2. Here we avoid floating‑point by using an
 * integer square‑root: floor(n*phi) = (n + floor(sqrt(5*n*n)))/2.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x <= 2^64-1 */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = ((uint64_t)1 << 32); /* sqrt(2^64-1) < 2^32 */
    uint64_t mid, r = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if ((unsigned __int128)mid * mid <= x) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */
    uint64_t idx = (uint64_t)n;
    uint64_t m   = (idx - 1) / 3 + 1;      /* block index (n in the formula) */
    uint64_t j   = (idx - 1) % 3 + 1;      /* position inside the block (1,2,3) */

    /* floor(m*phi) using integer arithmetic */
    uint64_t sq = isqrt_uint64((unsigned __int128)5 * m * m);
    uint64_t b  = (m + sq) / 2;            /* floor(m*phi) */

    uint64_t val = b + 3 * m;              /* base value for j=3 */
    if (j == 1) val -= 2;
    else if (j == 2) val -= 1;

    return (seq_t)val;
}
