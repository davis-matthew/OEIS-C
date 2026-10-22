/*
 * A375090  Omit the trailing 4 from the terms of the Lucas-Lehmer sequence
 *          A003010.
 * https://oeis.org/A375090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(n-1)^2 + 8*a(n-1) + 1 for n > 0 a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u)
        return (seq_t)0;
    seq_t term = (seq_t)1;          /* a(1) */
    if (n == 1u)
        return term;
    for (idx_t i = 2; i <= n; ++i) {
#ifdef __cplusplus
        /* Use seq_t arithmetic (works for built‑in unsigned types
           and for arbitrary‑precision types).  Overflow for
           built‑in types beyond the 64‑bit limit is acceptable,
           as the tests only check terms that fit in seq_t. */
        term = term * term * 10 + term * 8 + 1;
#else
        /* In C, use 128‑bit intermediate to avoid overflow
           up to the point where the value no longer fits.
           The cast to seq_t truncates the result when it overflows. */
        unsigned __int128 tmp = (unsigned __int128)term * term;
        tmp = tmp * 10 + (unsigned __int128)term * 8 + 1;
        term = (seq_t)tmp;
#endif
    }
    return term;
}
