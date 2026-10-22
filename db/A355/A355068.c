/*
 * A355068  Square array read by upwards antidiagonals: T(n,k) = k-th digit
 *          after the decimal point in decimal expansion of 1/n, for n >= 1
 *          and k >= 1.
 * https://oeis.org/A355068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = digit k of the decimal expansion of 1/r, * where (r , k)
 *            are the row and column indices obtained by * reading the array
 *            T(r,k)=k‑th digit of 1/r in upward antidiagonals
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the index s of the upward antidiagonal that contains the nth entry.
   s >= 2 and satisfies (s-1)*s/2 >= n, while (s-2)*(s-1)/2 < n. */
static inline idx_t find_s(idx_t n) {
    idx_t low  = 2;
    idx_t high = 4;                 /* start with a small window */

    /* Expand window until it surely contains the solution. */
    while (((__int128)high * (high - 1)) / 2 < n) {
        high <<= 1;
    }
    low = high >> 1;                /* previous value of high */

    /* Binary search within the window. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid - 1)) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;                     /* minimal s satisfying the condition */
}

/* Sequence entry a(n). */
seq_t a(idx_t n) {
    /* Identify the anti‑diagonal (s) and the position (k) within it. */
    idx_t s   = find_s(n);
    idx_t prev = ((idx_t)((__int128)(s - 2) * (s - 1)) / 2); /* sum before diagonal s */
    idx_t k   = n - prev;                 /* 1‑based position inside diagonal s */
    idx_t r   = s - k;                     /* row number = denominator */

    if (r == 1) return (seq_t)0;          /* 1/1 = 1.000… → all digits 0 */

    /* Compute the k‑th decimal digit of 1/r via integer long division. */
    idx_t rem = 1 % r;
    idx_t digit = 0;

    for (idx_t i = 1; i <= k; ++i) {
        __int128 tmp = (__int128)rem * 10;
        digit = (idx_t)(tmp / r);
        rem   = (idx_t)(tmp % r);
    }
    return (seq_t)digit;
}
