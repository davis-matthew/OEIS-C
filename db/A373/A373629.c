/*
 * A373629  a(n) = sum of all numbers whose binary expansion is n bits long,
 *          starts and ends with a 1 bit, and contains no 00 bit pairs.
 * https://oeis.org/A373629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) + 3*a(n-2) - 6*a(n-3) - 4*a(n-4)
 * Initial terms: a(1)=1, a(2)=3, a(3)=12, a(4)=39
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; below offset irrelevant */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)39;

    seq_t a1 = 1;   /* a(n-4)  */
    seq_t a2 = 3;   /* a(n-3)  */
    seq_t a3 = 12;  /* a(n-2)  */
    seq_t a4 = 39;  /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = a4 * 3 + a3 * 3 - a2 * 6 - a1 * 4;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
