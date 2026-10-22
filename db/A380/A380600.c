/*
 * A380600  Irregular table T(n, k), n > 0, k = 1..A000005(n) read by rows:
 *          the n-th row lists the numbers of the form n * (d-1) / d with d a
 *          positive divisor of n.
 * https://oeis.org/A380600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A380600:
   a(n) is the n-th term of the flattened table
   T(m,d) = m*(d-1)/d where d runs over the divisors of m in increasing order.
   The function iterates over rows until the cumulative number of divisors
   reaches n, then finds the k‑th divisor of that row by trial division.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"  /* provides divisor_count_u64(uint64_t) */

/* Helper to count divisors of a 64‑bit integer.
   This wrapper uses the supplied utility function. */
static inline idx_t tau_u64(uint64_t m) {
    return (idx_t)divisor_count_u64(m);
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, but defensive */

    idx_t cumulative = 0;   /* sum of divisor counts for rows < m */
    uint64_t m = 0;         /* current row number (1‑based) */

    /* Find the row m such that the n-th element lies in it. */
    while (1) {
        ++m;
        idx_t tau = tau_u64(m);
        if (cumulative + tau >= n) break;
        cumulative += tau;
    }

    /* Position of the element inside the found row. */
    idx_t k = n - cumulative;           /* 1‑based within the row */

    /* Find the k‑th divisor of m by trial division. */
    idx_t divisor = 1;                  /* placeholder */
    idx_t count = 0;
    for (uint64_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            ++count;
            if (count == k) {
                divisor = d;
                break;
            }
        }
    }

    /* Compute T(m,divisor) = m * (divisor-1) / divisor.
       Intermediate arithmetic is performed in 64‑bit unsigned integers
       to avoid overflow when seq_t is narrow (e.g., uint8_t). */
    uint64_t dd   = divisor;
    uint64_t mm   = m;
    uint64_t divq = mm / dd;
    uint64_t divr = mm % dd;
    uint64_t res64 = divq * (dd - 1) + divr * (dd - 1) / dd;

    return (seq_t)res64;
}
