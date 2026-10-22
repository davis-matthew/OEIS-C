/*
 * A368276  Number of nonnegative representations of n = w*x + y*z with max(w,
 *          x) < min(y, z) and w <= x <= y <= z.
 * https://oeis.org/A368276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) of non‑negative integers
 *            with * w <= x < y <= z and w*x + y*z = n
 * Range:     u8 n <= 599, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) using 128‑bit multiplication to avoid overflow */
    idx_t low = 0, high = (idx_t)1 << 32;          /* high^2 >= 2^64 > n */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Upper bound for the small variables w, x is sqrt(n) because w*x <= n */
    idx_t limit = isqrt_u64(n);

    for (idx_t w = 0; w <= limit; ++w) {
        for (idx_t x = w; x <= limit; ++x) {
            unsigned __int128 prod = (unsigned __int128)w * x;
            if (prod > n) break;              /* further x only increases prod */

            if (prod == 0) {                  /* handle w==0 separately to avoid overflow */
                // prod == 0 -> r == n, same code below
            }

            idx_t r = n - (idx_t)prod;
            if (r == 0) continue;             /* y*z cannot be 0 because y > x >= 0 */

            idx_t minY = x + 1;
            idx_t sqrt_r = isqrt_u64(r);

            if (minY > sqrt_r) continue;      /* no possible y */

            for (idx_t y = minY; y <= sqrt_r; ++y) {
                if (r % y == 0) {
                    /* z = r / y is automatically >= y because y <= sqrt_r */
                    ++count;
                }
            }
        }
    }

    return count;
}
