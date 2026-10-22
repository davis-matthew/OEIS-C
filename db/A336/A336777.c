/*
 * A336777  a(n) is the least number of repetitions such that the result of
 *          the repeated execution of the division f <- f/n started at f=1
 *          produces 0, when the division is performed using 32-bit single
 *          precision floats according to the IEEE 754 standard.
 * https://oeis.org/A336777
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = 1 + the number of times the assignment f <- f / n must be
 *            performed, starting from f = 1.0f, until f becomes zero in
 *            single‑precision IEEE 754 arithmetic
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)    /* the sequence is defined only for n ≥ 2 */
        return 0;

    float f   = 1.0f;          /* current value of f      */
    float nf  = (float)n;       /* n cast to float         */
    seq_t cnt = 0;              /* division counter        */

    /* Repeatedly divide until the result underflows to zero. */
    while (f != 0.0f) {
        f = f / nf;    /* single‑precision division */
        ++cnt;
    }

    /* The OEIS definition counts the final division that produces 0
       as an extra step, so add one to the raw count. */
    return cnt + 1;
}
