/*
 * A259212  A total of n married couples, including a mathematician M and his
 *          wife W, are to be seated at the 2n chairs around a circular table.
 *          M and W are the first couple allowed to choose chairs, and they
 *          choose two chairs next to each other. The sequence gives the
 *          number of ways of seating the remaining...
 * https://oeis.org/A259212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n-1)! * Sum_{j=0}^{n-1} (-1)^j * binomial(2*n-j-2, j) * (n-j-1)!
 *  for n > 1, and a(1)=a(2)=a(3)=0.
 *  The implementation computes the sum by a simple recurrence in O(n) time.
 */

#include <stdint.h>
#include <stdlib.h>

/* Multiplication via 128‑bit intermediate to avoid overflow when seq_t is u64. */
static inline seq_t mul128(seq_t x, seq_t y)
{
    return (seq_t)((unsigned __int128)x * y);
}

/* Factorial of m (m <= n-1) for general seq_t. */
static seq_t factorial_u(idx_t m)
{
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        f = mul128(f, (seq_t)i);
    return f;
}

/* Sequence A259212 */
seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)0;

    /* (n-1)! */
    seq_t fact = factorial_u(n - 1);

    /* B = binomial(2*n-j-2, j), init j=0 => B=1 */
    seq_t B = (seq_t)1;
    /* F = (n-j-1)!, init j=0 => F=fact */
    seq_t F = fact;

    /* sum = Σ term_j, start with term_0 = B*F = fact */
    seq_t sum = F;  /* term_0  */

    for (idx_t j = 0; j < n - 1; ++j) {
        /* Update B to B_{j+1}:
         * B_{j+1} = B_j * (2*n-2*j-2) / (2*n-j-2) * (2*n-2*j-3) / (j+1)
         */
        seq_t tmp = mul128(B, (seq_t)(2 * n - 2 * j - 2));
        tmp /= (seq_t)(2 * n - j - 2);
        tmp = mul128(tmp, (seq_t)(2 * n - 2 * j - 3));
        tmp /= (seq_t)(j + 1);
        B = tmp;

        /* Update F to F_{j+1} = (n-j-1)! / (n-j-1) */
        F /= (seq_t)(n - j - 1);

        /* term_{j+1} = B * F */
        seq_t term = mul128(B, F);

        /* Add or subtract according to sign */
        if ( ((j + 1) & 1) == 0 )   /* even index -> positive */
            sum += term;
        else
            sum -= term;
    }

    /* a(n) = (n-1)! * sum */
    return mul128(fact, sum);
}
