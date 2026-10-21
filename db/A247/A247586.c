/*
 * A247586  Number of acute triangles with integer sides less than or equal to
 *          n.
 * https://oeis.org/A247586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (a,b,c) with 1 ≤ a ≤ b ≤ c ≤ n
 *            such that a+b>c and a²+b²>c² (count of acute triangles with
 *            integer sides ≤ n)
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(n)) for uint64_t */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0, b = 1ULL << 62;
    while (b > n) b >>= 2;
    while (b) {
        if (n >= x + b) {
            n -= x + b;
            x = (x >> 1) + b;
        } else {
            x >>= 1;
        }
        b >>= 2;
    }
    return x;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* offset is 1 */
    seq_t total = 0;

    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t b = a; b <= n; ++b) {
            /* maximum possible c limited by n and triangle inequality */
            uint64_t maxc1 = n;
            uint64_t maxc2 = a + b - 1;           /* a+b>c  →  c ≤ a+b-1 */
            /* ensure not to underflow when a=b=1 */
            uint64_t sumsq = (uint64_t)a * (uint64_t)a +
                            (uint64_t)b * (uint64_t)b; /* a² + b²  */
            uint64_t maxc3 = isqrt_u64(sumsq - 1); /* a²+b² > c²  →  c < sqrt(a²+b²) */
            uint64_t maxc = maxc1 < maxc2 ? maxc1 : maxc2;
            if (maxc3 < maxc) maxc = maxc3;
            if (maxc >= (uint64_t)b) {
                total += (seq_t)(maxc - (uint64_t)b + 1);
            }
        }
    }
    return total;
}
