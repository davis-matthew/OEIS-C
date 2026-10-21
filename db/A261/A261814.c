/*
 * A261814  a(n) = smallest initial value s such that n is in the Collatz
 *          trajectory of s (s is not regarded as part of its trajectory).
 * https://oeis.org/A261814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest s such that n is in the Collatz trajectory of s
 *            (the starting value s itself is not counted as part of its
 *            trajectory)
 * Range:     u8 n <= 24, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for offset 0, return 0 */

    seq_t target = (seq_t)n;
    idx_t limit = 2 * n;          /* upper bound for all cases */

    /* Search s from 2 upwards; s=1 is never minimal because 1 never reaches n>1 */
    for (idx_t s = 2; s <= limit; ++s) {
        seq_t x = (seq_t)s;

        /* Apply first Collatz step; we do not test the starting value itself */
        if ((x & 1) == 0)
            x >>= 1;
        else
            x = x * 3 + 1;
        if (x == target)
            return (seq_t)s;

        /* Continue until the sequence terminates in 1-cycle */
        while (x != 1) {
            if ((x & 1) == 0)
                x >>= 1;
            else
                x = x * 3 + 1;
            if (x == target)
                return (seq_t)s;
        }
    }

    /* According to the theory this point is never reached. */
    return 0;
}
