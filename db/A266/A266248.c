/*
 * A266248  Decimal representation of the middle column of the "Rule 9"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence: a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3)   for n > 6
 * Initial values: a(0)=1, a(1)=2, a(2)=5, a(3)=10, a(4)=21, a(5)=43, a(6)=86
 * This is equivalent to a(n) = floor(65*2^n/48).
 */

seq_t a(idx_t n)
{
    /* Handle the first 7 terms directly */
    static const seq_t init[7] = {1, 2, 5, 10, 21, 43, 86};

    if (n < 7)
        return init[n];

    /* Start from a(4), a(5), a(6) as the last three known terms */
    seq_t a_nm3 = init[4];   /* a(n-3) */
    seq_t a_nm2 = init[5];   /* a(n-2) */
    seq_t a_nm1 = init[6];   /* a(n-1) */

    /* Iterate up to the desired index */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t a_n = (a_nm1 * 2) + a_nm2 - (a_nm3 * 2);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1; /* a(n) after the loop ends */
}
