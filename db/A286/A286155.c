/*
 * A286155  Square array A(n,k) read by antidiagonals, A(n,n) = -n, otherwise,
 *          if n > k, A(n,k) = T(n XOR k,k), else A(n,k) = T(n,n XOR k), where
 *          T(n,k) is sequence A000027 considered as a two-dimensional table
 *          and XOR is bitwise-xor (A003987).
 * https://oeis.org/A286155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array A(i,j) * where
 *            A(n,n) = -n, * A(n,k) = T(n XOR k, k) for n > k, * A(n,k) = T(n,
 *            n XOR k) for n < k, * and T(n,k) = n + (n+k-2)(n+k-1)/2 \ (the
 *            two‑dimensional layout of A000027). * The index n (1‑based) is
 *            mapped to coordinates (i,j) by antidiagonals
 * Range:     i8 n <= 70, i16 n <= 9872, i32 n <= 9872, i64 n <= 9872,
 *            i128 n <= 9872, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t T(idx_t n, idx_t k)
{
    __int128 s = (__int128)n + k - 2;                 /* s = n + k - 2  */
    __int128 tri = s * (s + 1) / 2;                  /* triangular(s)  */
    __int128 val = tri + n;                          /* T = n + tri    */
    return (seq_t)val;
}

seq_t a(idx_t n)
{
    /* Find the diagonal d (i+j = d) that contains the n‑th element.
       The total number of elements up to diagonal d is (d-1)d/2. */
    idx_t low = 2, high = 1ULL << 32;               /* 2^32 > sqrt(2*2^64) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 tri = (__int128)(mid - 1) * mid / 2;
        if (tri < n) low = mid + 1;
        else high = mid;
    }
    idx_t d = low;                                   /* diagonal index */

    /* Number of elements before diagonal d. */
    __int128 before = (__int128)(d - 2) * (d - 1) / 2;
    idx_t offset = (idx_t)((__int128)n - before - 1); /* 0‑based offset on the diagonal */

    /* Coordinates (i,j) on the diagonal: i increases from 1 upwards. */
    idx_t i = offset + 1;
    idx_t j = d - offset - 1;

    /* Compute A(i,j) according to the definition. */
    if (i == j) {
        return -(seq_t)i;
    } else if (i > j) {
        idx_t xor_ = i ^ j;
        return T(xor_, j);
    } else {
        idx_t xor_ = i ^ j;
        return T(i, xor_);
    }
}
