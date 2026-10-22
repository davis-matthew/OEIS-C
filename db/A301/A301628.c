/*
 * A301628  Numbers not divisible by 2, 3 or 5 (A007775) with digital root 7.
 * https://oeis.org/A301628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n>5; Initial values: a(1)=7,
 *            a(2)=43, a(3)=61, a(4)=79, a(5)=97
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: sequence is defined for n >= 1 only */
    if (n < 1)
        return 0; /* Undefined for n < 1, but return 0 for safety */

    /* Initial 5 terms – fits in any seq_t */
    static const seq_t init[5] = {7, 43, 61, 79, 97};

    if (n <= 5)
        return init[n - 1];

    /* Use a sliding window of the last five terms */
    seq_t a_minus_5 = init[0];
    seq_t a_minus_4 = init[1];
    seq_t a_minus_3 = init[2];
    seq_t a_minus_2 = init[3];
    seq_t a_minus_1 = init[4];
    seq_t a_curr = 0;

    /* Compute terms iteratively up to n */
    for (idx_t i = 6; i <= n; ++i) {
        a_curr = a_minus_1 + (a_minus_4 - a_minus_5);
        /* Shift the window */
        a_minus_5 = a_minus_4;
        a_minus_4 = a_minus_3;
        a_minus_3 = a_minus_2;
        a_minus_2 = a_minus_1;
        a_minus_1 = a_curr;
    }

    return a_curr;
}
