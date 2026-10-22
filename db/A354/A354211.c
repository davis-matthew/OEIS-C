/*
 * A354211  a(n) is the numerator of Sum_{k=0..n} 1 / (2*k+1)!.
 * https://oeis.org/A354211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{k=0..n} 1/(2*k+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute gcd of two seq_t values (built‑in % works for the big integer class). */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t U = 1;          /* U(n) = sum_{k=0..n} (2n+1)!/(2k+1)!    */
    seq_t T = 1;          /* T(n) = (2n+1)!                         */

    for (idx_t k = 1; k <= n; ++k) {
        /* mult = (2k)*(2k+1) fits comfortably in 64 bits          */
        idx_t small = (2 * k) * (2 * k + 1);
        seq_t mult = (seq_t)small;

        U = mult * U + (seq_t)1;  /* Update U(n)  = mult*U(n-1)+1          */
        T = mult * T;            /* Update T(n)  = mult*T(n-1)             */
    }

    seq_t g = seq_gcd(U, T);   /* Reduce the fraction U/T               */
    return U / g;
}
