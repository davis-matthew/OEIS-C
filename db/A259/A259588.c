/*
 * A259588  Denominators of the other-side convergents to e.
 * https://oeis.org/A259588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A259588 – denominators of the other‑side convergents to e
 *
 *  Sequence definition:
 *    Let the continued fraction of e be [2; 1,2,1,1,4,1,1,6, …] where for i≥1
 *    a_i = 1   if i%3 ≠ 2
 *         = 2*((i+1)/3)   if i%3 = 2
 *    Let q_{-1}=0, q_0=1 and for i≥1
 *          q_i = a_i*q_{i-1} + q_{i-2}          (normal convergent denominators)
 *
 *    For the “other‑side” convergents define
 *          Q_0 = 1
 *          Q_i = q_{i-1}*(a_i+1) + q_{i-2}      for i≥1
 *
 *  The function below returns a(n)=Q_n.
 */

seq_t a(idx_t n)
{
    /*  Handle n = 0 separately */
    if (n == 0) return (seq_t)1;

    seq_t q_im2 = (seq_t)0;  /* q_{-1} */
    seq_t q_im1 = (seq_t)1;  /* q_0  */

    seq_t Q = (seq_t)1;     /* Q_0 – value for n = 0 (unused after the first loop) */

    idx_t i;
    for (i = (idx_t)1; i <= n; ++i)
    {
        /* compute the i‑th partial quotient a_i */
        seq_t a_i;
        if (i % 3 == 2)
            a_i = (seq_t)2 * (seq_t)((i + 1) / 3);
        else
            a_i = (seq_t)1;

        /* other‑side denominator Q_i */
        seq_t Q_i = q_im1 * (a_i + (seq_t)1) + q_im2;

        /* normal convergent denominator q_i (for next iteration) */
        seq_t q_i = a_i * q_im1 + q_im2;

        /* if we've reached the requested index, return the result */
        if (i == n) return Q_i;

        /* shift indices for next loop iteration */
        q_im2 = q_im1;
        q_im1 = q_i;
        Q = Q_i;   /* keep latest Q value (unused after return) */
    }

    /* unreachable but keeps the compiler happy */
    return Q;
}
