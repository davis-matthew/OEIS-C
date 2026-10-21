/*
 * A267003  Decimal representation of the n-th iteration of the "Rule 83"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 25*a(n-2) - 152*a(n-4) + 128*a(n-6)   (n > 5)
   Initial values: a(0)=1, a(1)=5, a(2)=8, a(3)=111, a(4)=64, a(5)=1919 */

seq_t a(idx_t n)
{
    /* first six terms (constants from the OEIS entry) */
    const seq_t first[6] = { (seq_t)1, (seq_t)5, (seq_t)8,
                            (seq_t)111, (seq_t)64, (seq_t)1919 };

    if (n < 6)
        return first[(idx_t)n];

    /* rolling buffer of the last six computed values  */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = first[i];

    for (idx_t k = 6; k <= n; ++k)
    {
        /* indices modulo 6 */
        int idx   = (int)(k % 6);
        int i2    = (int)((k - 2) % 6);      /* a(k-2)   */
        int i4    = (int)((k - 4) % 6);      /* a(k-4)   */
        /* a(k-6) has the same remainder as k */
        seq_t t1 = (seq_t)25 * buf[i2];
        seq_t t2 = (seq_t)152 * buf[i4];
        seq_t t3 = (seq_t)128 * buf[idx];   /* a(k-6) = a(k)   */

        /* compute a(k) = t1 - t2 + t3  (no underflow because the result is
           always non‑negative for this sequence) */
        buf[idx] = t1 + t3 - t2;
    }

    return buf[(int)(n % 6)];
}
