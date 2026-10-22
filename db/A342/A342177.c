/*
 * A342177  Product of first n Motzkin numbers.
 * https://oeis.org/A342177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} Motzkin(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Motzkin numbers satisfy: M(0)=1, M(1)=1 and
   M(k) = ((2k+1)M(k‑1) + (3k‑3)M(k‑2)) / (k+2) for k≥2. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* M(0) and M(1) */
    seq_t m_prev2 = (seq_t)1;     /* M(0) */
    seq_t m_prev1 = (seq_t)1;     /* M(1) */

    seq_t product = (seq_t)1;     /* a(0) */

    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* Compute Motzkin number M(k) using the recurrence */
        seq_t m = (((seq_t)(2 * k + 1)) * m_prev1 +
                   ((seq_t)(3 * k - 3)) * m_prev2) /
                  (k + 2);

        product *= m;             /* update prefix product */

        /* Shift previous values for next iteration */
        m_prev2 = m_prev1;
        m_prev1 = m;
    }

    return product;
}
