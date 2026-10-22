/*
 * A394865  a(n) is the size of the largest subset A of {1, ..., n} such that
 *          x does not divide 2*y for each pair of distinct elements x, y in
 *          A.
 * https://oeis.org/A394865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * A394865(n) = A050292(n) – A050292(floor(n/3)) * A050292(n)
 *            counts integers 1..n with an even 2‑valuation * (i.e. the
 *            exponent of 2 in the prime factorisation is even). * The
 *            construction of a maximal subset is to take all * m with
 *            floor(n/3) < m ≤ n and v₂(m) even
 * Range:     u8 n <= 574, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t, if needed by compiler */
#include "../utils/count_even.h"

seq_t a(idx_t n)
{
    seq_t fe = count_even(n);          /* A050292(n)   */
    seq_t fm = count_even(n / 3);      /* A050292(floor n/3) */
    return fe - fm;
}
