/*
 * A268833  Square array A(n, k) = A101080(k, A003188(n+A006068(k))), read by
 *          descending antidiagonals, where A003188 is the binary Gray code,
 *          A006068 is its inverse, and A101080(x,y) gives the Hamming
 *          distance between binary expansions of x and y.
 * https://oeis.org/A268833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = the Hamming‐distance A101080 between the column index k
 *            and * the Gray code value A003188(n + A006068(k)), * where (n,k)
 *            is obtained by reading the square array A(n,k) along *
 *            descending antidiagonals (i.e. all pairs with n+k = constant,
 *            with k * descending). Thus a(0)=0, a(1)=0, a(2)=1,...
 * Range:     u8 n <= 9348, u16 n <= 9348, u32 n <= 9348, u64 n <= 9348,
 *            u128 n <= 9348, big any n
 * Helpers:   gray, gray_inverse, popcount_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gray.h"
#include "../utils/gray_inverse.h"
#include "../utils/popcount_uint64.h"

/* Count 1‑bits of a 64‑bit value */

/* Sequence A268833 */
seq_t a(idx_t n)
{
    /* Find the diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2. */
    idx_t t = 0;
    /* For n up to 10 000 this loop runs at most ~140 times. */
    while (((t + 1U) * (t + 2U)) / 2U <= n)
        ++t;

    /* Index of the first term on this diagonal. */
    idx_t prev = t * (t + 1U) / 2U;

    /* Position within the diagonal (0 <= offset <= t). */
    idx_t offset = n - prev;

    /* Coordinates on the array: i = offset,   j = t - offset. */
    idx_t i = offset;
    idx_t j = t - offset;

    /* Compute A(i,j) = Hamming distance between j and Gray(i + GrayInverse(j)). */
    uint64_t inv_j   = gray_inverse(j);
    uint64_t sum     = i + inv_j;
    uint64_t gray_sum = gray(sum);
    uint64_t diff    = j ^ gray_sum;
    unsigned int hamming = popcount_uint64(diff);

    return (seq_t)hamming;
}
