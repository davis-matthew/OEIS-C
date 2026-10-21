/*
 * A272172  Triangle read by rows: T(n,k) in which row n lists the first n
 *          terms of A000203 in reverse order.
 * https://oeis.org/A272172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = σ(r - (n - r(r-1)/2) + 1), where r =
 *            ceil((sqrt(8n+1)-1)/2) and σ is the sum-of-divisors function.
 *            The function constructs the triangle of σ-values in reverse
 *            order and returns the n‑th term in row‑major order
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(idx_t n) {
    if (n == 0) return 0;
    idx_t low = 0, high = (idx_t)0xFFFFFFFFULL;  /* sqrt(UINT64_MAX) < 2^32 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* Sum of divisors of m (returned in seq_t) */
static seq_t sum_of_divisors(idx_t m) {
    seq_t sum = 0;
    idx_t i, lim = isqrt_u64(m);
    for (i = 1; i <= lim; ++i) {
        if (m % i == 0) {
            seq_t j = (seq_t)(m / i);
            sum += (seq_t)i;
            if (i != j) sum += j;
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset is 1, behaviour for n<1 is undefined */

    /* Find r such that r(r+1)/2 >= n (smallest such r) */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) r <<= 1;      /* exponential search */
    idx_t low = r >> 1, high = r;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) high = mid;
        else                                             low = mid + 1;
    }
    r = high;

    /* Position within the r‑th row */
    idx_t prev = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t pos = n - prev;                     /* 1 <= pos <= r */
    idx_t m   = r - pos + 1;                  /* index fed to σ */

    return sum_of_divisors(m);
}
