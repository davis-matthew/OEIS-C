/*
 * A396719  Volume (in unit cubes) of the level-n additive cube fractal in
 *          which a central cube of side 2*Pell(n-1) is surrounded by six
 *          copies of the previous level (silver-ratio scaling).
 * https://oeis.org/A396719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1;
   a(n) = 5 * a(n-1) + s(n)^3 + s(n-1)^3   for n >= 2
   s(1) = 1;
   s(n) = 2 * Pell(n-1),  Pell(0)=0, Pell(1)=1,
   Pell(k) = 2 * Pell(k-1) + Pell(k-2) for k >= 2
   (These are the Pell numbers A000129.) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* value below offset (not used) */
    /* Initial values for n = 1 */
    seq_t a_prev   = (seq_t)1;            /* a(1)   */
    seq_t s_prev   = (seq_t)1;            /* s(1)   */
    seq_t p_prevprev = (seq_t)0;          /* Pell(0) */
    seq_t p_prev     = (seq_t)1;          /* Pell(1) */

    if (n == 1) return a_prev;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* s(i) = 2 * Pell(i-1) */
        seq_t s_curr  = (seq_t)2 * p_prev;

        /* compute cubes */
        seq_t s_curr_cu = s_curr * s_curr * s_curr;
        seq_t s_prev_cu = s_prev * s_prev * s_prev;

        /* a(i) = 5*a(i-1) + s(i)^3 + s(i-1)^3 */
        seq_t a_curr = ((seq_t)5 * a_prev) + s_curr_cu + s_prev_cu;

        /* update Pell for next iteration: Pell(i) = 2*Pell(i-1)+Pell(i-2) */
        seq_t p_next = ((seq_t)2 * p_prev) + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = p_next;

        /* update s and a for next loop */
        s_prev = s_curr;
        a_prev = a_curr;
    }

    return a_prev;
}
