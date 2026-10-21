/*
 * A275334  Number of simple labeled graphs on n vertices that have at least
 *          one vertex of odd degree and at least one vertex of even degree.
 * https://oeis.org/A275334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^C(n,2) - 2*2^C(n-1,2) if n is even a(n) = 2^C(n,2) -
 *            2^C(n-1,2) if n is odd (C(n,2) = n*(n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Returns the number of labeled simple graphs on n vertices
   that have at least one odd‑degree vertex and at least one even‑degree vertex. */
seq_t a(idx_t n)
{
    /* 2^C(n,2) */
    seq_t pow_n2  = pow2((idx_t)binomial(n, 2));

    /* 2^C(n-1,2) */
    seq_t pow_n1_2 = pow2((idx_t)binomial(n - 1, 2));

    if ((n & 1) == 0) {          /* even n */
        seq_t twice = pow_n1_2 + pow_n1_2;   /* 2 * 2^C(n-1,2) */
        return pow_n2 - twice;
    } else {                     /* odd n */
        return pow_n2 - pow_n1_2;
    }
}
