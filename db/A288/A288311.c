/*
 * A288311  Number of steps, reduced mod n, to reach 1 in the Collatz 3x+1
 *          problem, or -1 if 1 is never reached.
 * https://oeis.org/A288311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006577(n) mod n (number of Collatz steps to 1, reduced
 *            mod n)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    idx_t current = n;
    idx_t steps = 0;
    while (current != 1) {
        if ((current & 1) == 0)      /* even */
            current >>= 1;
        else                         /* odd */
            current = 3 * current + 1;
        steps++;
    }
    return (seq_t)(steps % n);
}
