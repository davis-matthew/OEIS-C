/*
 * A289121  a(n) = (8 - 2*n + 11*n^2 - 6*n^3 + n^4)/4.
 * https://oeis.org/A289121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8 - 2*n + 11*n^2 - 6*n^3 + n^4)/4
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1  (a(1) is the first term) */

seq_t a(idx_t n)
{
    /* Handle the defined offset (a(1) is the first term).  For n < 1
       we simply return 0; the function is only required for n ≥ 1. */
    if (n < 1)
        return 0;

    /* Use a type that can hold the intermediate 128‑bit result.
       The expression is a polynomial in n and the exact answer is
       guaranteed to be an integer.  The intermediate 128‑bit value
       will not overflow for all n that produce a result fitting in
       seq_t. */
    __int128 nn = n;

    __int128 val = (__int128)8
                   - (__int128)2 * nn
                   + (__int128)11 * nn * nn
                   - (__int128)6 * nn * nn * nn
                   + (__int128)nn * nn * nn * nn;

    val /= 4;                   /* division is exact */

    return (seq_t)val;          /* cast to the build‑specific type */
}
