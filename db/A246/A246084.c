/*
 * A246084  Paradigm shift sequence for (1,2) production scheme with
 *          replacement.
 * https://oeis.org/A246084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 136, u64 n <= 277,
 *            u128 n <= 559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246084
   Recurrence: a(n) = 3 * a(n-7) for all n ≥ 26
   Initial values: a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7,
   a(8)=8, a(9)=9, a(10)=10, a(11)=12, a(12)=15, a(13)=18, a(14)=21,
   a(15)=24, a(16)=28, a(17)=32, a(18)=36, a(19)=45, a(20)=54,
   a(21)=63, a(22)=72, a(23)=84, a(24)=96, a(25)=112 */
seq_t a(idx_t n)
{
    /* Initial terms 1 … 25 */
    static const seq_t init[25] = {
        1,  2,  3,  4,  5,  6,  7,
        8,  9, 10, 12, 15, 18, 21,
        24, 28, 32, 36, 45, 54, 63,
        72, 84, 96,112
    };

    /* Return known values for n ≤ 25 */
    if (n <= 25)
        return init[n - 1];

    /* For n ≥ 26 use the recurrence
       a(n) = 3 * a(n-7), applied repeatedly until the index
       falls into the initial segment.  Let k be the number of
       applications and m the reduced index. */
    idx_t k = (n - 26) / 7 + 1;          /* k ≥ 1 */
    idx_t m = n - 7 * k;                 /* 1 ≤ m ≤ 25 */

    /* Compute 3^k in seq_t */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow3 *= (seq_t)3;

    return pow3 * init[m - 1];
}
