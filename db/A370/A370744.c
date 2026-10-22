/*
 * A370744  a(n) is the greatest Fibonacci number f such that f AND n = f
 *          (where AND denotes the bitwise AND operator).
 * https://oeis.org/A370744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the largest Fibonacci number f ≤ n such that (f & n) == f
 *            (i.e., all 1-bits of f are also 1-bits of n)
 * Range:     u8 n <= 376, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to the sequence type for comparisons and bitwise ops */
    const seq_t N = (seq_t)n;

    seq_t best = 0;          /* store the running best Fibonacci satisfying the condition */
    seq_t a0 = 0;            /* first Fibonacci number */
    seq_t a1 = 1;            /* second Fibonacci number */

    /* iterate over Fibonacci numbers using the standard recurrence a[i+1] = a[i] + a[i-1] */
    while (a0 <= N) {
        /* check the AND condition */
        if ((a0 & N) == a0) {
            best = a0;
        }
        /* compute next Fibonacci number */
        seq_t next = a0 + a1;
        a0 = a1;
        a1 = next;
    }

    return best;
}
