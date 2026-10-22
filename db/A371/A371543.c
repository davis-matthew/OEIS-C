/*
 * A371543  G.f. A(x) satisfies A(x) = (1 + x*A(x) / (1+x))^4.
 * https://oeis.org/A371543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n-1,n-k) *
 *            binomial(4*k+4,k)/(k+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via recurrence on the combinatorial factors to avoid large
   intermediate values.  The implementation uses 128‑bit integers for
   intermediate arithmetic to remain exact on the 64‑bit build. */

#include <stdint.h>
#include <stdbool.h>

/* Assume seq_t and idx_t are defined in the build environment. */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Running sum (signed to allow subtraction) */
    signed __int128 sum = 0;

    /* bin1 = binom(n-1, k-1) for current k (starts with k=1). */
    seq_t bin1 = 1;           /* binom(n-1,0) */
    /* g = binom(4*k+4, k)/(k+1) for current k (starts with k=1, g(0)=1). */
    seq_t g = 1;          /* g(0) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update g to g(k) from g(k-1).  Here cm = k-1. */
        idx_t cm = k - 1;
        unsigned __int128 num = 1;
        num *= (unsigned __int128)(4 * cm + 5); /* 4k+5 */
        num *= (unsigned __int128)(4 * cm + 6); /* 4k+6 */
        num *= (unsigned __int128)(4 * cm + 7); /* 4k+7 */
        num *= (unsigned __int128)(4 * cm + 8); /* 4k+8 */

        unsigned __int128 denom = 1;
        denom *= (unsigned __int128)(3 * cm + 5); /* 3k+5 */
        denom *= (unsigned __int128)(3 * cm + 6); /* 3k+6 */
        denom *= (unsigned __int128)(3 * cm + 7); /* 3k+7 */
        denom *= (unsigned __int128)(cm + 2);     /* k+1  (since cm = k-1) */

        unsigned __int128 prod_tmp = (unsigned __int128)g * num;
        prod_tmp /= denom;
        g = (seq_t)prod_tmp;   /* now g = binom(4k+4,k)/(k+1) */

        /* Current term: bin1 * g */
        unsigned __int128 prod = (unsigned __int128)bin1 * (unsigned __int128)g;

        /* Add or subtract based on parity of (n-k) */
        if (((n - k) & 1) == 0)
            sum += (__int128)prod;
        else
            sum -= (__int128)prod;

        /* Prepare bin1 for next k (k+1).  Only if k < n. */
        if (k < n)
        {
            unsigned __int128 prod2 = (unsigned __int128)bin1 * (unsigned __int128)(n - k);
            prod2 /= (unsigned __int128)k;
            bin1 = (seq_t)prod2;
        }
    }

    return (seq_t)sum;   /* sum is guaranteed non‑negative for n ≥ 1 */
}
