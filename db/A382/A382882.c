/*
 * A382882  Triangle read by rows: T(n, k) = k^ord(n, k) where ord(n, k) is
 *          the p-adic order if n and k >= 2, 1 if k = 1, and 0^n if k = 0.
 * https://oeis.org/A382882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) element of the triangle read by rows, where
 *            T(r, c) = c^e and e is the largest integer such that c^e divides
 *            r (i.e., e = v_c(r)). For k = 0 or 1 the value is 1 (except the
 *            single entry a(0) = 0). The sequence is indexed starting at n =
 *            0
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_isqrt_u64(idx_t n)
{
    /* Computes floor(sqrt(n)) without using floating point. */
    idx_t x = 0;
    idx_t bit = 1ULL << 62;          /* 2^62 is the largest power of two <= sqrt(UINT64_MAX) */
    while (bit != 0) {
        if ((x | bit) <= n) {
            x |= bit;
        }
        bit >>= 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Special case for the very first element. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t m = 1 + 8 * n;
    idx_t sqrt_m = floor_isqrt_u64(m);
    idx_t r = (sqrt_m - 1) / 2;

    /* Adjust r to guarantee the inequality holds. */
    while (r * (r + 1) / 2 > n) {
        --r;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row. */
    idx_t c = n - r * (r + 1) / 2;   /* 0 <= c <= r */

    /* Handle special values. */
    if (c == 0 || c == 1) {
        return (seq_t)1;
    }

    /* Compute the exponent e = v_c(r). */
    idx_t temp = r;
    idx_t e = 0;
    while (temp % c == 0) {
        temp /= c;
        ++e;
    }

    /* Compute c^e in seq_t type. */
    seq_t result = 1;
    seq_t base = (seq_t)c;
    for (idx_t i = 0; i < e; ++i) {
        result *= base;
    }
    return result;
}
