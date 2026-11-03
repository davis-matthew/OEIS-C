/*
 * A283845  Square array read by antidiagonals: T(1,1) = T(1,2) = T(2,1) =
 *          T(2,2) = 1; thereafter T(m,n) = min {T(m,n-2) + T(m,n-1), T(m-2,n)
 *          + T(m-1,n), T(m-2,n-2) + T(m-1,n-1)}.
 * https://oeis.org/A283845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(max(i,j)) where (i,j) are the antidiagonal
 *            coordinates of index n (i = k, j = d-k+1 with d = smallest
 *            integer such that d*(d+1)/2 ≥ n)
 * Range:     u8 n <= 91, u16 n <= 300, u32 n <= 1128, u64 n <= 4371,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 94 terms through n = 6619 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset 1 – not used, but safe */

    /* Find the diagonal d such that d*(d+1)/2 ≥ n */
    const idx_t MAX_D = ((idx_t)1 << 33) - 1;   /* sufficient for any 64‑bit n */
    idx_t low = 1, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 < n && high < MAX_D)
        high <<= 1;
    if (high > MAX_D)
        high = MAX_D;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                         /* diagonal number (1‑based) */

    /* Position k inside the diagonal: 1 ≤ k ≤ d */
    idx_t prev = (idx_t)(((__uint128_t)(d - 1) * d) / 2);   /* elements before diagonal */
    idx_t k = n - prev;                   /* index along the diagonal */

    /* Antidiagonal coordinates: i = k, j = d - k + 1 */
    idx_t j = d - k + 1;
    idx_t m = (k > j) ? k : j;            /* max(i,j) */

    return fib(m);                        /* Fibonacci(m) */
}
