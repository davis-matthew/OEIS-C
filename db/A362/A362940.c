/*
 * A362940  Consider the Collatz trajectory from n to 1, assuming the Collatz
 *          conjecture is true. Then a(n) = number of terms in the trajectory
 *          that are greater than 1 and congruent to 1 mod 3. If n never
 *          reaches 1, set a(n) = -1.
 * https://oeis.org/A362940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the Collatz trajectory of n that are
 *            >1 and congruent to 1 modulo 3
 * Range:     u8 n <= 26, u16 n <= 8946, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A362940. */
seq_t a(idx_t n)
{
    /* n == 1 yields 0 by definition. */
    if (n <= 1)
        return 0;

    seq_t count = 0;
    seq_t cur = (seq_t)n;

    /* Iterate until the trajectory reaches 1. */
    while (cur > 1)
    {
        /* Count terms >1 that are ≡1 (mod 3). */
        if (cur % 3 == 1)
            count++;

        /* Collatz step: halve if even, else 3x+1. */
        if ((cur & 1) == 0)
            cur >>= 1;          /* cur = cur / 2   */
        else
            cur = cur * 3 + 1;  /* cur = 3*cur + 1 */
    }

    return count;
}
