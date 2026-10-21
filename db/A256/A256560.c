/*
 * A256560  Triangle read by rows, sums of 2 distinct nonzero squares plus
 *          sums of 2 distinct nonzero cubes: T(n,k) = n^2 + k^2 + n^3 + k^3,
 *          1 <= k <= n-1.
 * https://oeis.org/A256560
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r is the smallest integer with (r-1)*r/2 >=
 *            n-1 and k = n - ( (r-2)*(r-1)/2 + 1 ). T(r, k) = r^2 + k^2 + r^3
 *            + k^3
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic and works for any idx_t/seq_t combination. */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for 128‑bit values (returns floor(sqrt(x))) */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0;
    uint64_t hi = (uint64_t)1 << 34;          /* sqrt(2^67) < 2^34 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Below the offset (n < 2) the sequence is undefined; return 0. */
    if (n < 2)
        return (seq_t)0;

    /* 1 + 8*(n-1) fits in 128 bits, its square root fits in 64 bits. */
    unsigned __int128 val = (unsigned __int128)8 * (n - 1) + 1;
    uint64_t sq = isqrt_u128(val);

    /* Compute r = ceil((1 + sqrt(val))/2) */
    uint64_t r = (sq + 1) >> 1;
    if ((unsigned __int128)sq * sq != val)
        ++r;   /* sqrt was not exact → need the next integer */

    /* Compute k = n - ((r-2)*(r-1)/2 + 1) */
    unsigned __int128 prev = (unsigned __int128)(r - 2) * (r - 1) / 2;
    uint64_t k = n - ((uint64_t)(prev) + 1);

    /* Compute T(r, k) = r^2 + k^2 + r^3 + k^3 */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;

    seq_t res = sr * sr + sk * sk + sr * sr * sr + sk * sk * sk;
    return res;
}
