/*
 * A395079  a(n) = Sum_{k=0..n} n^k * binomial(n + 1, k + 1) * binomial(2*n +
 *          2, k) / (n + 1).
 * https://oeis.org/A395079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * binomial(n + 1, k + 1) * binomial(2*n
 *            + 2, k) / (n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A395079.  The implementation uses a recurrence
 * to generate each summand iteratively.  For integer seq_t types the
 * update is performed using an unsigned 128‑bit intermediate (available on
 * GCC/Clang) to avoid overflow; for big‑integer seq_t types (e.g. boost::multiprecision
 * cpp_int) the arithmetic is done directly with seq_t operations.
 */
#ifndef __cplusplus
seq_t a(idx_t n)
{
    seq_t sum  = (seq_t)1;   /* a(0) = 1 */
    seq_t term = (seq_t)1;   /* T0 */

    for (idx_t k = 0; k < n; ++k)
    {
        /* Using unsigned 128‑bit intermediate for safety on 64‑bit seq_t. */
        __uint128_t num = (__uint128_t)term * n
                         * (n - k)
                         * (2 * n + 2 - k);
        __uint128_t den = (k + 2) * (k + 1);
        term = (seq_t)(num / den);   /* exact division */
        sum += term;
    }
    return sum;
}
#else /* C++ implementation with compile‑time type selection */
#include <type_traits>
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;   /* a(0) = 1 */
    seq_t term = (seq_t)1;  /* T0 */

    for (idx_t k = 0; k < n; ++k)
    {
        if constexpr (std::is_integral_v<seq_t>) {
            /* 128‑bit intermediate for integral seq_t. */
            unsigned __int128 num = (unsigned __int128)term * n
                                   * (n - k)
                                   * (2 * n + 2 - k);
            unsigned __int128 den = (k + 2) * (k + 1);
            term = (seq_t)(num / den);
        } else {
            /* Big‑integer seq_t: perform exact arithmetic with seq_t. */
            seq_t num = term * n;
            num = num * (n - k);
            num = num * (2 * n + 2 - k);
            seq_t den = (seq_t)(k + 2) * (seq_t)(k + 1);
            term = num / den;   /* exact division */
        }
        sum += term;
    }
    return sum;
}
#endif
