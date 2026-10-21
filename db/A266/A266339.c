/*
 * A266339  G.f. = b(2)^2*b(4)/(x^5+x^4-x^3-x^2-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266339: G.f. = b(2)^2*b(4)/(x^5+x^4-x^3-x^2-x+1)
   where b(k) = (1-x^k)/(1-x).
   The coefficients satisfy the linear recurrence
   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)-a(n-5)   for n >= 6,
   with initial values
   a(0)=1, a(1)=4, a(2)=9, a(3)=18, a(4)=33, a(5)=56. */

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 4, 9, 18, 33, 56};

    if (n < 6)
        return init[n];

    /* last[0..4] holds a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) for the next
       iteration.  Initialize them for n = 6. */
    seq_t last[5] = { init[1], init[2], init[3], init[4], init[5] };
    idx_t i;

    for (i = 6; i <= n; ++i)
    {
        /* a(i) = a(i-1)+a(i-2)+a(i-3) - (a(i-4)+a(i-5)) */
        seq_t sum3 = last[4] + last[3] + last[2];
        seq_t sum2 = last[1] + last[0];
        seq_t newval = sum3 - sum2;

        /* shift window for next step */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = newval;
    }

    return last[4];
}
