/*
 * A241193  a(n) = Sum_{k=1..n}
 *          ((3*n-k-1)/(2*n-k))*(3*n-k-2)!/((n-1)!*(n-1)!*(n-k)!).
 * https://oeis.org/A241193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(1)=1, a(2)=11, a(3)=181 For n ≥
 *            4: a(n) = [2*(32*n³-131*n²+187*n-94)*a(n-1)
 *            +3*(-86*n³+721*n²-1896*n+1617)*a(n-2)
 *            -18*(2*n-5)*(3*n-8)*(3*n-7)*a(n-3)] /((2*n-1)*(n-1)²)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes A241193(a(n)) for n ≥ 1, with support for any seq_t type.
   Intermediate arithmetic uses signed __int128 to avoid overflow
   when seq_t is a 64‑bit unsigned integer. For built‑ins that
   do not support __int128, the compiler will still compile this
   since the intermediate type is only used as a temporary. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 – return 0 */
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 181;

    /* previous three terms: a(n-3), a(n-2), a(n-1) */
    seq_t prev3 = 1;   /* a(1) */
    seq_t prev2 = 11;  /* a(2) */
    seq_t prev1 = 181; /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        __int128 ii  = (__int128)i;
        __int128 i2  = ii * ii;
        __int128 i3  = i2 * ii;

        /* polynomial coefficients, all performed in 128‑bit */
        __int128 poly1 = 32 * i3 - 131 * i2 + 187 * ii - 94;
        __int128 poly2 = -86 * i3 + 721 * i2 - 1896 * ii + 1617;
        __int128 poly3 = (2 * ii - 5) * (3 * ii - 8) * (3 * ii - 7);

        /* compute numerator terms */
        __int128 term1 = 2 * poly1 * (__int128)prev1;   /* a(n-1) */
        __int128 term2 = 3 * poly2 * (__int128)prev2;   /* a(n-2) */
        __int128 term3 = 18 * poly3 * (__int128)prev3;  /* a(n-3) */

        __int128 numer = term1 + term2 - term3;
        __int128 denom = (2 * ii - 1) * (ii - 1) * (ii - 1);

        cur = (seq_t)(numer / denom);    /* exact division */

        /* shift the window for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
