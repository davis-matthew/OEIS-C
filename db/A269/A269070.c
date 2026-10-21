/*
 * A269070  Number of n X 3 binary arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling two not more than once.
 * https://oeis.org/A269070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 31*a(n-2) + 24*a(n-3) + 21*a(n-4) - 18*a(n-5)
 *            - 9*a(n-6) with initial values a(1)=7, a(2)=27, a(3)=123,
 *            a(4)=537, a(5)=2343, a(6)=10167. Sequence is defined for n ≥ 1
 *            (offset 1). The function returns a(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle a(1) … a(6) directly. */
    static const seq_t base[7] = {0, 7, 27, 123, 537, 2343, 10167};

    if (n == 0)                  /* undefined but return 0 for safety */
        return 0;
    if (n <= 6)
        return base[n];

    /* Variables holding the last six terms.  Initially they are a(1)…a(6). */
    seq_t a1 = 7,    /* a(n-6)  */
          a2 = 27,   /* a(n-5)  */
          a3 = 123,  /* a(n-4)  */
          a4 = 537,  /* a(n-3)  */
          a5 = 2343, /* a(n-2)  */
          a6 = 10167,/* a(n-1)  */
          term = 0;  /* a(n)    */

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute a(i) using the recurrence. */
        seq_t plus = a6;
        plus *= 10;

        seq_t tmp = a4;
        tmp *= 24;
        plus += tmp;

        tmp = a3;
        tmp *= 21;
        plus += tmp;

        seq_t minus = a5;
        minus *= 31;

        tmp = a2;
        tmp *= 18;
        minus += tmp;

        tmp = a1;
        tmp *= 9;
        minus += tmp;

        term = plus - minus;   /* plus ≥ minus for all n ≥ 1 */

        /* Shift window: drop a1, move others, insert new term. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = term;
    }

    return term;
}
