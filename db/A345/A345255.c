/*
 * A345255  Carter and Reyes sequence for the game of countably infinitely
 *          many hats.
 * https://oeis.org/A345255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = -1; a(1)= 1; a(2)=0; a(3)=0; a(4)=2; a(5)=1; a(6)=2; For
 *            n ≥ 7: if n mod 8 ∈ {1,2,3,4,5,6} then a(n) = a(n mod 8); if n
 *            mod 8 = 0 then a(n) = a(2·⌊n/8⌋) + 2·[a(2·⌊n/8⌋)≠0]; if n mod 8
 *            = 7 then a(n) = a(2·⌊n/8⌋+1) + 2·[a(2·⌊n/8⌋+1)≠0]; (square
 *            brackets denote the Iverson bracket, i.e. 1 if...
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t init[7] = {
        (seq_t)-1, (seq_t)1, (seq_t)0, (seq_t)0,
        (seq_t)2,  (seq_t)1, (seq_t)2
    };

    /* base values for n < 7 */
    if (n < 7)
        return init[n];

    /* n % 8; use bitwise for speed (because 8 is a power of two) */
    idx_t r = n & 7;                     /* equivalent to n % 8 */

    /* for residues 1..6 we simply copy the corresponding small value */
    if (r >= 1 && r <= 6)
        return init[r];

    /* recursion for residues 0 and 7 */
    if (r == 0) {                       /* n mod 8 == 0 */
        idx_t m = (n / 8) * 2;          /* 2·⌊n/8⌋          */
        seq_t sub = a(m);
        return sub + 2 * (sub != 0);
    } else {                            /* n mod 8 == 7 */
        idx_t m = (n / 8) * 2 + 1;      /* 2·⌊n/8⌋ + 1      */
        seq_t sub = a(m);
        return sub + 2 * (sub != 0);
    }
}
