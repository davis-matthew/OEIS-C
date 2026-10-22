/*
 * A309131  Triangle read by rows: T(n, k) = (n - k)*prime(1 + k), with 0 <= k
 *          < n.
 * https://oeis.org/A309131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r, k) where r = smallest integer such that r(r+1)/2 ≥
 *            n, k = n - r(r-1)/2 - 1; T(r, k) = (r - k) * prime(1 + k)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* offset starts at 1 */

    /* Find the row r containing the n-th term. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++r;
        cumulative += r;                    /* cumulative = r(r+1)/2 */
    }

    /* Position within the row. */
    idx_t prefix_prev = cumulative - r;     /* = r(r-1)/2 */
    idx_t k = n - prefix_prev - 1;          /* 0‑based index in the row */

    /* Compute the term: (r - k) * prime(1 + k). */
    seq_t prime_val = (seq_t)get_prime(k + 1);
    return (seq_t)(r - k) * prime_val;
}
