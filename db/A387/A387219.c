/*
 * A387219  Product of the first n terms of the paperfolding sequence A034947,
 *          or the Kronecker symbol (-1|n!).
 * https://oeis.org/A387219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = product of the first n terms of the paperfolding sequence
 *            A034947, which equals (-1)^{A255070(n)}. Each term of that
 *            sequence is +1 if the odd part of its index is congruent to 1
 *            (mod 4), and –1 if the odd part is congruent to 3 (mod 4)
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the product of the first n terms of A034947. */
seq_t a(idx_t n)
{
    seq_t prod = 1;              /* the product stays ±1 */
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t odd = i;           /* remove all factors of 2 */
        while ((odd & 1) == 0)
            odd >>= 1;
        /* odd part modulo 4: if it is 3, flip the sign */
        if ((odd & 3) == 3)
            prod = -prod;
    }
    return prod;
}
