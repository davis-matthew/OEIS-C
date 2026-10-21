/*
 * A247128  Positive numbers that are congruent to {0,5,9,13,17} mod 22.
 * https://oeis.org/A247128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*floor((n-1)/5) + [5,9,13,17,22][(n-1)%5]
 * Range:     u8 n <= 58, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but return 0 */

    idx_t zero_based = n - 1;      /* careful: n>=1 so no wrap-around */
    idx_t block = zero_based / 5;  /* how many full blocks of 5 */
    idx_t pos   = zero_based % 5;  /* position inside the block */

    seq_t base = (seq_t)22 * (seq_t)block;

    /* choose the offset according to the position in the block */
    switch (pos) {
        case 0: return base + 5;
        case 1: return base + 9;
        case 2: return base + 13;
        case 3: return base + 17;
        case 4: return base + 22;
        default: return 0; /* unreachable */
    }
}
