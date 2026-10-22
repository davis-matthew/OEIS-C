/*
 * A378966  Area of the unique primitive Pythagorean triple (x,y,z) such that
 *          (x-y+z)/2 is A002315(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A378966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002315(n)*(A002315(n)-1)*(2*A002315(n)-1) with
 *            A002315(0)=1, A002315(1)=7,
 *            A002315(n)=6*A002315(n-1)-A002315(n-2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = A002315(n) by the linear recurrence */
    seq_t t0 = 1;   /* t(0) */
    seq_t t1 = 7;   /* t(1) */
    seq_t t_val;

    if (n == 0) {
        t_val = t0;
    } else if (n == 1) {
        t_val = t1;
    } else {
        seq_t prev2 = t0;
        seq_t prev1 = t1;
        seq_t cur;
        for (idx_t i = 2; i <= n; ++i) {
            /* cur = 6 * prev1 - prev2  */
            cur = (prev1 << 2) + (prev1 << 1);  /* 6*prev1 */
            cur -= prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        t_val = prev1;
    }

    /* a(n) = t*(t-1)*(2*t-1) */
    seq_t one = 1;
    seq_t t_minus1 = t_val - one;
    seq_t two_t = t_val << 1;           /* 2*t */
    seq_t two_t_minus1 = two_t - one;
    seq_t prod = t_val * t_minus1;
    return prod * two_t_minus1;
}
