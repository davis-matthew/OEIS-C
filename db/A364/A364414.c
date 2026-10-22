/*
 * A364414  Numbers k with the property that the second part of the symmetric
 *          representation of sigma(k) is an octagon of width 1 and one of its
 *          vertices is also the central vertex of the first valley of the
 *          largest Dyck path of the diagram.
 * https://oeis.org/A364414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n-th term (n ≥ 1) of OEIS A364414. The sequence
 *            consists of all integers >9 that are 3 mod 6 but do not end in
 *            the digit 5
 * Range:     u8 n <= 32, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Example of first few terms:
        n  a(n)
        1   21
        2   27
        3   33
        4   39
        5   51
        6   57
        7   63
        8   69
        9   81
        10  87
        ...
*/

seq_t a(idx_t n)
{
    if (n == 0) {               /* undefined for n=0 – return 0 as safe value */
        return 0;
    }

    idx_t cand = 21;            /* first term */
    idx_t cnt  = 1;             /* count of valid terms found so far */

    while (cnt < n) {
        cand += 6;              /* successive integers congruent to 3 mod 6 */
        if ((cand % 10) != 5) { /* skip numbers ending in 5 */
            ++cnt;
        }
    }
    return (seq_t)cand;
}
