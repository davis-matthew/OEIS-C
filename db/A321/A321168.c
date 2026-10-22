/*
 * A321168  Maximum number of squarefree conjugates of a ternary word of
 *          length n.
 * https://oeis.org/A321168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)-a(24) are given. For all n >= 1, a(n) = n except for n ∈
 *            {5,7,9,10,14,17} where a(n)=3,5,4,6,10,11 respectively. For n ≥
 *            18, a(n) = n
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1; behaviour for n < 1 is arbitrary */
    switch (n) {
        case 5:  return (seq_t)3;
        case 7:  return (seq_t)5;
        case 9:  return (seq_t)4;
        case 10: return (seq_t)6;
        case 14: return (seq_t)10;
        case 17: return (seq_t)11;
        default: return (seq_t)n;
    }
}
