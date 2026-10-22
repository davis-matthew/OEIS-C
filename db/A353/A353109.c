/*
 * A353109  Array read by antidiagonals: A(n, k) is the digital root of n*k
 *          with n >= 0 and k >= 0.
 * https://oeis.org/A353109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   a(n) = digital root of (i*j), where i + j =
 *            floor((sqrt(8n+1)-1)/2) and i = n - i*(i+1)/2 (triangular index)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the largest s such that s*(s+1)/2 <= n */
static uint64_t find_s(idx_t n)
{
    uint64_t low = 0, high = 1;
    /* Exponential search to find an upper bound */
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    low = high >> 1;
    /* Binary search between low+1 and high for the exact s */
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    uint64_t s = find_s(n);                     /* diagonal number */
    uint64_t base = (__uint128_t)s * (s + 1) / 2;/* triangular number T(s) */
    uint64_t i = n - base;                      /* row index within diagonal */
    uint64_t j = s - i;                         /* column index */

    if (i == 0 || j == 0)                       /* product is 0 */
        return (seq_t)0;

    /* digital root of i*j: reduce modulo 9 first to avoid overflow */
    uint64_t mod9 = ((i % 9) * (j % 9)) % 9;
    return (seq_t)(mod9 == 0 ? 9 : mod9);
}
