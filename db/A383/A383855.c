/*
 * A383855  The n-th term of the sequence is k after every k*(k+1)/2
 *          occurrences of 1, with multiple values following a 1 listed in
 *          order.
 * https://oeis.org/A383855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383855: a(1)=1.
 * An explicit simulation is used.  Let count_ones be the total
 * number of 1's that have appeared so far.  Whenever a 1 is output,
 * its position causes count_ones to increase by one.  Then for every
 * integer k ≥ 2 for which count_ones is a multiple of the triangular
 * number T_k = k*(k+1)/2, the value k is output immediately after
 * that 1, in increasing order of k.  The process repeats until the
 * desired index n is reached. */

seq_t a(idx_t n)
{
    if (n == 0)                  /* outside the OEIS offset: never used   */
        return (seq_t)0;

    idx_t count_ones = 0;   /* how many 1's we have seen               */
    idx_t pos        = 1;   /* current position in the sequence          */

    while (1) {
        /* output a 1 */
        if (pos == n)                /* first term of this block             */
            return (seq_t)1;
        pos++;

        count_ones++;                /* one more '1' has just occurred        */

        /* After this 1, output every k such that
           count_ones is a multiple of T_k = k*(k+1)/2.               */
        for (idx_t k = 2; ; ++k) {
            seq_t Tk = (seq_t)k * (k + 1) / 2;   /* triangular number      */
            if (Tk > count_ones)               /* no larger k possible   */
                break;

            if ((seq_t)count_ones % Tk == 0) { /* threshold reached     */
                if (pos == n)
                    return (seq_t)k;
                pos++;
            }
        }
    }
}
