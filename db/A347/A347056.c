/*
 * A347056  Triangle read by rows: T(n,k) = (n+1)*(n+2)*(k+3)*binomial(n,k)/6,
 *          0 <= k <= n.
 * https://oeis.org/A347056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (r+1)*(r+2)*(k+3)*binomial(r,k)/6 * where n is the
 *            flattened index of the triangle T(n,k). * The triangle is read
 *            row by row: row 0 has 1 entry, * row 1 has 2 entries, etc. * *
 *            Implementation uses the supplied binomial() routine. * If the
 *            compiler supports 128‑bit integers, intermediate *...
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline seq_t mul_seq(seq_t a, seq_t b) {
#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    /* Use 128‑bit intermediate when available */
    __int128 t = (__int128)a * (__int128)b;
    return (seq_t)t;
#else
    return a * b;
#endif
}

seq_t a(idx_t n) {
    /* Find the row r and column k such that the flattened
       triangle index n lies in row r, column k. */
    idx_t r = 0;
    idx_t sum = 1;                  /* number of elements up to row 0 */

    while (sum <= n) {
        ++r;
        sum += r + 1;               /* add (r+1) elements for the next row */
    }

    idx_t prev_sum = sum - (r + 1); /* elements before row r */
    idx_t k = n - prev_sum;         /* column index within row r */

    /* Compute T(r,k) = (r+1)*(r+2)*(k+3)*binomial(r,k)/6 */
    seq_t res = binomial(r, k);
    res = mul_seq(res, (seq_t)(r + 1));
    res = mul_seq(res, (seq_t)(r + 2));
    res = mul_seq(res, (seq_t)(k + 3));
    res = res / 6;                  /* integer division is exact */

    return res;
}
