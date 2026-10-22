/*
 * A375371  Expansion of 1/( (1 + x)^2 * (1 - x*(1 + x)^2) ).
 * https://oeis.org/A375371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Recurrence: a(n) = -a(n-1) + 3*a(n-2) + 6*a(n-3) + 4*a(n-4) +
 *            a(n-5), with a(0)=1, a(1)=-1, a(2)=4, a(3)=-1, a(4)=11
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)-1;
    if (n == 4) return (seq_t)11;

    /* Start with the five preceding values for i = 5 */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)-1;  /* a(1) */
    seq_t a2 = (seq_t)4;   /* a(2) */
    seq_t a3 = (seq_t)-1;  /* a(3) */
    seq_t a4 = (seq_t)11;  /* a(4) */

    /* Iterate from i = 5 up to n, computing each term from the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t new_val = -a4 + (seq_t)3 * a3 + (seq_t)6 * a2 + (seq_t)4 * a1 + a0;
        /* Shift values: drop the oldest and add the newest */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_val;
    }

    /* After the loop, a4 holds a(n) */
    return a4;
}
