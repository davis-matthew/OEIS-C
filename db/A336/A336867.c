/*
 * A336867  Numbers k such that k! does not have distinct prime
 *          multiplicities.
 * https://oeis.org/A336867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 5, a(n) = n + 5; a(1)=3, a(2)=5, a(3)=7, a(4)=8, a(5)=9.
 *            This is the sequence of all non‑negative integers except
 *            0,1,2,4,6,10
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is guaranteed to be ≥ 1 */
    if (n <= 5) {
        switch ((unsigned)n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)5;
            case 3: return (seq_t)7;
            case 4: return (seq_t)8;
            case 5: return (seq_t)9;
            default: return (seq_t)0; /* unreachable */
        }
    } else {
        return (seq_t)n + (seq_t)5;
    }
}
