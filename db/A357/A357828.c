/*
 * A357828  a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n,3*k)|.
 * https://oeis.org/A357828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A357828:  a(n) = Sum_{k=0..floor(n/3)} |Stirling1(n,3*k)|
 * Equivalently, a(n) is the number of permutations of n elements
 * whose number of cycles is congruent to 0 (mod 3).
 * Recurrence:
 *   Let a_r(n) be the number of permutations of n with cycle count ≡ r (mod 3).
 *   Then for r = 0,1,2:
 *       a_r(n) = a_{r-1}(n-1) + (n-1) * a_r(n-1),
 *   with a_0(0)=1, a_1(0)=0, a_2(0)=0.
 *   The sequence sought is a_0(n).
 */

seq_t a(idx_t n)
{
    /* Handle the offset directly */
    if (n == 0)
        return (seq_t)1;

    /* a0 = a_0, a1 = a_1, a2 = a_2 */
    seq_t a0 = (seq_t)1, a1 = (seq_t)0, a2 = (seq_t)0;
    seq_t prev0, prev1, prev2, mult;

    for (idx_t i = 1; i <= n; ++i) {
        /* Store previous values */
        prev0 = a0;
        prev1 = a1;
        prev2 = a2;

        /* (i-1) as seq_t for multiplication */
        mult = (seq_t)(i - 1);

        /* Update for next n */
        a0 = prev2 + mult * prev0;  /* a_0(n) */
        a1 = prev0 + mult * prev1;  /* a_1(n) */
        a2 = prev1 + mult * prev2;  /* a_2(n) */
    }

    return a0;  /* a_0(n) is the desired sequence value */
}
