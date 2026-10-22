/*
 * A328082  Triangle read by rows: columns are Fibonacci numbers F_{2i+1}
 *          shifted downwards.
 * https://oeis.org/A328082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(2*(r-1 - col)+1) where r =
 *            ceil((sqrt(8*n+1)-1)/2), col = n - r*(r-1)/2 - 1
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 300, u64 n <= 1128,
 *            u128 n <= 4371, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned 64‑bit value. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t prod = (__uint128_t)mid * mid;
        if (prod <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Compute Fibonacci(n) with additions only.  For 64‑bit seq_t this works
   correctly up to n = 92, which is sufficient for all terms that fit
   in 64 bits.  Larger n values overflow, but those terms are not checked. */
static seq_t fib_iter(idx_t n)
{
    seq_t a = 0, b = 1;
    while (n > 0) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
        --n;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – undefined for 0 */

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    if ((r * (r + 1)) / 2 < n) r++;

    /* Number of elements before this row. */
    idx_t prev = (r * (r - 1)) / 2;

    /* Column index (0‑based) within the row. */
    idx_t col = n - prev - 1;

    /* Position counting down from the top of the row. */
    idx_t i = r - 1 - col;

    /* Fibonacci index (odd). */
    idx_t fib_index = 2 * i + 1;

    return fib_iter(fib_index);
}
