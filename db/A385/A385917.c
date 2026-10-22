/*
 * A385917  Hexagonal numbers that are one-sixth of another hexagonal number.
 * https://oeis.org/A385917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385917: Hexagonal numbers that are one‑sixth of another hexagonal number.
 * Computation is based on the Pell equation X² – 6Y² = –5, where for every term
 * X = 4m – 1, Y = 4k – 1 and H_k = k·(2k – 1). The initial solution is
 * (X₁,Y₁) = (7,3) corresponding to k = 1. Subsequent solutions are obtained
 * by the linear transformation
 *
 *   X_{n+1} = 49·X_n + 120·Y_n
 *   Y_{n+1} = 20·X_n + 49·Y_n
 *
 * which is M² applied to the fundamental unit of X² – 6Y² = 1.
 * The n‑th term (with a(1)=0) is therefore
 *
 *   a(1) = 0
 *   a(n) = (k·(2k – 1))  for n ≥ 2,  where k = (Y_{n-1} + 1)/4.
 */

seq_t a(idx_t n)
{
    /* a(1) is defined as 0. */
    if (n == 1)
        return 0;

    /* j = n – 1 gives index j of the solution (X_j , Y_j) with j ≥ 1.
       The first solution (j = 1) is (X₁, Y₁) = (7, 3). */
    idx_t j = n - 1;

    seq_t X = 7;   /* X₁ */
    seq_t Y = 3;   /* Y₁ */

    /* Generate the (j‑1)‑th additional solution using the recurrence. */
    for (idx_t i = 1; i < j; ++i)
    {
        seq_t X_new = 49 * X + 120 * Y;
        seq_t Y_new = 20 * X + 49 * Y;
        X = X_new;
        Y = Y_new;
    }

    /* From Y = 4k – 1 obtain the index k of the hexagonal number. */
    seq_t k = (Y + 1) / 4;

    /* Compute the hexagonal number H_k = k·(2k – 1). */
    seq_t h = k * (2 * k - 1);
    return h;
}
