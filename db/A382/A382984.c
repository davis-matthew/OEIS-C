/*
 * A382984  Coefficient of x^3 in expansion of (x+1) * (x+4) * ... *
 *          (x+3*n-2).
 * https://oeis.org/A382984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x³ in ∏_{j=1}^{n} (x + 3·j – 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let c[k] be the coefficient of xᵏ in the product of the first n–1 factors. */
/* Then after multiplying by (x + aₙ) with aₙ = 3·n – 2: */
/*   new_c[3] = aₙ * c[3] + c[2] */
/*   new_c[2] = aₙ * c[2] + c[1] */
/*   new_c[1] = aₙ * c[1] + c[0] */
/*   new_c[0] = aₙ * c[0] */
/* Initial state (product of zero factors): c[0] = 1, c[1] = c[2] = c[3] = 0. */

seq_t a(idx_t n)
{
    seq_t c0 = 1;   /* coefficient of x⁰ */
    seq_t c1 = 0;   /* coefficient of x¹ */
    seq_t c2 = 0;   /* coefficient of x² */
    seq_t c3 = 0;   /* coefficient of x³ */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t a_n = ((seq_t)3 * i) - ((seq_t)2);

        seq_t nc3 = a_n * c3 + c2;
        seq_t nc2 = a_n * c2 + c1;
        seq_t nc1 = a_n * c1 + c0;
        seq_t nc0 = a_n * c0;

        c3 = nc3;
        c2 = nc2;
        c1 = nc1;
        c0 = nc0;
    }

    return c3;   /* coefficient of x³ in the n‑fold product */
}
