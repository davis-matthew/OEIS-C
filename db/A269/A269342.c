/*
 * A269342  a(n) = (n + 1)*(2*n + 1)*(4*n + 9)/3.
 * https://oeis.org/A269342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(2*n + 1)*(4*n + 9)/3
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for intermediate values so that the
       product never overflows for any n that keeps the result in
       seq_t. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long prod = (nn + 1ULL) *
                              (2ULL * nn + 1ULL) *
                              (4ULL * nn + 9ULL) /
                              3ULL;
    return (seq_t)prod;
}
