/*
 * A343084  a(n) is the smallest number that is the sum of n positive squares
 *          in four ways.
 * https://oeis.org/A343084
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 24 for n ≥ 8, special values: a(2)=1105, a(3)=129,
 *            a(4)=52, a(5)=53, a(6)=36, a(7)=37
 * Range:     u8 none, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2, offset is 2 */
    switch (n) {
        case 2: return (seq_t)1105;
        case 3: return (seq_t)129;
        case 4: return (seq_t)52;
        case 5: return (seq_t)53;
        case 6: return (seq_t)36;
        case 7: return (seq_t)37;
        default: ;
    }
    return (seq_t)n + (seq_t)24;
}
