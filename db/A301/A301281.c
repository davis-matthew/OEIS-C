/*
 * A301281  Numerator of proportion of the volume of a unit box in R^4 that
 *          can be filled by n disjoint symplectically embedded balls of equal
 *          radius.
 * https://oeis.org/A301281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=8, a(5)=9, a(6)=48, a(7)=224,
 *            a(n)=1 for all n≥8. (Numerator of the packing proportion.)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index; the sequence is defined for all n≥1 */
    if (n <= 7) {
        switch ((unsigned long long)n) {
            case 1:  case 2: return 1;
            case 3:                return 2;
            case 4:                return 8;
            case 5:                return 9;
            case 6:                return 48;
            case 7:                return 224;
        }
    }
    return 1;
}
