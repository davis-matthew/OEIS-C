/*
 * A253710  Second partial sums of tenth powers (A008454).
 * https://oeis.org/A253710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 59, u128 n <= 2440,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A253710 – Second partial sums of tenth powers.
 *  Offset: 1 (a(1)=1)
 *
 *  Formula (recurrence):
 *    a(n) = 2*a(n-1) - a(n-2) + n^10      for n ≥ 3
 *    a(1) = 1
 *    a(2) = 1026
 *
 *  The implementation uses only integer arithmetic supplied by the
 *  build system (seq_t is the sequence value type, idx_t is the index type).
 */

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n == 1) return (seq_t)1;

    /* Initial values: a(1) and a(2). */
    seq_t a0 = (seq_t)1;          /* a(n-2) for n=3 */
    seq_t a1 = (seq_t)1026;       /* a(n-1) for n=3 */

    /* Special case for n==2. */
    if (n == 2) return a1;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute i^10 using repeated multiplication. */
        seq_t pow = (seq_t)1;
        seq_t nseq = (seq_t)i;
        for (idx_t k = 0; k < 10; ++k) {
            pow *= nseq;
        }

        /* Apply the recurrence: next = 2*a1 - a0 + pow. */
        seq_t next = a1;
        next *= (seq_t)2;
        next -= a0;
        next += pow;

        /* Shift window for next iteration. */
        a0 = a1;
        a1 = next;
    }

    return a1;
}
