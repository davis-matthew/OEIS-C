/*
 * A254647  Fourth partial sums of eighth powers (A001016).
 * https://oeis.org/A254647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1+n)*(2+n)^2*(3+n)*(4+n)*(1 +4*n +n^2)*(21 -48*n
 *            +20*n^2 + 16*n^3 +2*n^4)/23760
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 86, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* GCD for unsigned __int128 */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0u) {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 denom = 23760u;
    unsigned __int128 prod = 1u;

    /* pre‑compute the six multiplicative factors */
    unsigned __int128 f[6];
    f[0] = nn;                                      /* n            */
    f[1] = nn + 1u;                                 /* n+1          */
    f[2] = (nn + 2u) * (nn + 2u);                  /* (n+2)^2      */
    f[3] = (nn + 3u) * (nn + 4u);                  /* (n+3)(n+4)   */
    f[4] = 1u + 4u * nn + nn * nn;                 /* 1+4n+n^2     */
    f[5] = 21u - 48u * nn + 20u * nn * nn            /* 21-48n+20n^2 */
           + 16u * nn * nn * nn
           + 2u * nn * nn * nn * nn;               /* +16n^3+2n^4  */

    for (int i = 0; i < 6; ++i) {
        unsigned __int128 fi = f[i];

        /* cancel common factors between the factor and the remaining denominator */
        unsigned __int128 g = gcd128(fi, denom);
        if (g != 1u) {
            fi /= g;
            denom /= g;
        }

        prod *= fi;

        /* after the multiplication, reduce the product with whatever of the
           denominator remains, to avoid unnecessary growth */
        g = gcd128(prod, denom);
        if (g != 1u) {
            prod /= g;
            denom /= g;
        }
    }

    /* some residue of the denominator could still remain (e.g., if all factors
       happened to be coprime with the denominator).  The entire product is
       guaranteed to be divisible by the original denominator, so a final
       division finishes the computation. */
    if (denom != 1u)
        prod /= denom;

    return (seq_t)prod;
}
