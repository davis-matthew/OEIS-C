/*
 * A306562  a(n) = 1 + 2 - 3 - 4 + 5 + 6 + 7 - 8 - 9 - 10 - 11 + 12 + 13 + 14
 *          + 15 + ... + (+-1)*n, where, after the 1st summand there is one
 *          plus, two minuses, three plusses, etc.
 * https://oeis.org/A306562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = 1 + Σ_{k=1..n} s_k · (k+1) where the integer k belongs to
 *            a block of consecutive integers; the block sizes are 1,2,3,4,...
 *            and the sign s_k is +1 for odd‑numbered blocks and –1 for
 *            even‑numbered blocks. The first block (size 1) contains k = 1
 *            and is a “+” block
 * Range:     i8 n <= 35, i16 n <= 1324, i32 n <= 9279, i64 n <= 9279,
 *            i128 n <= 9279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A306562 for any n ≥ 0.  Complexity is O(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t result = (seq_t)1;       /* start with the first summand 1 */

    /* group: 1,2,3,4,...   group_rem: remaining positions in current group */
    idx_t group = 1;
    idx_t group_rem = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* sign for current k: +1 if group is odd, –1 if even */
        seq_t term = (seq_t)(k + 1);
        if ((group & 1U) == 0)   /* even group -> negative sign */
            term = -term;

        result += term;

        /* advance to next element in the group */
        if (--group_rem == 0) {
            ++group;
            group_rem = group;
        }
    }

    return result;
}
