/*
 * A343658  Array read by antidiagonals where A(n,k) is the number of ways to
 *          choose a multiset of k divisors of n.
 * https://oeis.org/A343658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal reading of the triangle
 *            A(n,k)=C(d(n)+k-1,k), where d(n) is the divisor count of n. The
 *            code maps the linear index n to coordinates (n_val,k_val) of the
 *            triangle and returns binom(d(n_val)+k_val-1, k_val)
 * Range:     u8 n <= 125, u16 n <= 539, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find diagonal index s such that (s+1)(s+2)/2 >= n */
    idx_t s = 0;
    for (;;) {
        unsigned __int128 cum = ((unsigned __int128)(s + 1) * (s + 2)) / 2;
        if (cum >= n) break;
        ++s;
    }

    /* Number of terms before this diagonal */
    unsigned __int128 prev_total = ((unsigned __int128)s * (s + 1)) / 2;
    /* Zero‑based offset within the diagonal */
    idx_t offset = n - (idx_t)prev_total - 1;

    /* Coordinates of the triangle:
       n_val counts rows (starting from 1)
       k_val counts columns (starting from 0) */
    idx_t n_val = offset + 1;
    idx_t k_val = s - offset;

    /* Number of divisors of n_val */
    uint64_t d = divisor_count_u64((uint64_t)n_val);

    /* Compute binomial coefficient C(d + k_val - 1, k_val) */
    idx_t a_val = (idx_t)d + k_val - 1;
    return binom(a_val, k_val);
}
