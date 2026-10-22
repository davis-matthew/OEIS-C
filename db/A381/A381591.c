/*
 * A381591  Elimination order of the first person in a variation of the
 *          Josephus problem, where there are n people total and two people
 *          are skipped each time.
 * https://oeis.org/A381591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the elimination step at which person 1 is eliminated
   in the Josephus problem with step size 3 (two people skipped each
   time).  The simulation runs in O(n) time and uses only O(1)
   auxiliary storage. */
seq_t a(idx_t n)
{
    /* Handle the trivial case; the sequence is defined for n >= 1 */
    if (n == 0)
        return (seq_t)0;

    const idx_t k = 3;          /* step size (skip two people) */

    idx_t  m       = n;         /* current number of survivors   */
    idx_t  current = 1;         /* starting position for counting */
    idx_t  pos1    = 1;         /* current position of person 1   */
    seq_t  step    = 1;         /* elimination counter           */

    while (1)
    {
        /* Position (1‑based) of the next eliminated person */
        idx_t pos = ((current + k - 2) % m) + 1;

        if (pos == pos1)           /* person 1 is eliminated now   */
            return step;

        /* Update the stored position of person 1 if needed */
        if (pos < pos1)
            pos1--;

        /* Determine the counting start for the next round */
        if (pos < m)
            current = pos;        /* next after pos in the old circle */
        else
            current = 1;          /* wrap around if last was eliminated */

        /* Prepare for next iteration */
        m--;
        step++;
    }
}
