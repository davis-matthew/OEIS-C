/*
 * A377009  Number of odd terms in the Collatz trajectory of k = 4n-1 which
 *          are a new record high among its odd terms.
 * https://oeis.org/A377009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of odd terms in the Collatz trajectory of k = 4n-1
 *            which are a new record high among its odd terms. The first odd
 *            term (k itself) is not counted; only subsequent odd terms that
 *            exceed all previous odd terms are counted
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the above definition by simulating the Collatz
   trajectory of k = 4n-1.  For each odd term after the start we compute
   the next odd term by applying the 3x+1 rule and repeatedly dividing by 2
   until an odd number is obtained.  A counter of new record highs is
   maintained.  The algorithm runs in time linear in the length of the
   trajectory, which is sufficiently fast for n up to about 10 000. */

seq_t a(idx_t n)
{
    /* k = 4n - 1 */
    seq_t k = (seq_t)n * 4 - 1;

    /* current odd term in the trajectory */
    seq_t current = k;

    /* maximum odd seen so far (start with k, but not counted) */
    seq_t maxOdd = k;

    /* number of new record highs */
    seq_t count = 0;

    while (1)
    {
        /* compute the next odd term */
        seq_t y = current * 3 + 1;
        /* divide by 2 until odd */
        while ((y % 2) == 0)
            y >>= 1;

        /* if we reached 1, no more record highs possible */
        if (y == 1)
            break;

        /* count it if it is a new record high */
        if (y > maxOdd)
        {
            count += 1;
            maxOdd = y;
        }

        /* move to the next odd term */
        current = y;
    }

    return count;
}
