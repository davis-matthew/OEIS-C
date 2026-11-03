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
 * Validated: standard, pass, 86 terms through n = 9858 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uintptr_t, if needed by compiler */

static seq_t count_even(idx_t limit)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        /* trailing zeros of i; cnt of 2‑division */
        int tz = __builtin_ctzll((unsigned long long)i);
        if ((tz & 1) == 0)  /* even exponent of 2 */
            ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t fe = count_even(n);          /* A050292(n)   */
    seq_t fm = count_even(n / 3);      /* A050292(floor n/3) */
    return fe - fm;
}
