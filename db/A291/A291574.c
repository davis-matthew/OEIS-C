/*
 * A291574  The arithmetic function uhat(n,6,6).
 * https://oeis.org/A291574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-6) for n > 6 Initial values: a(1)=1, a(2)=0, a(3)=-1,
 *            a(4)=0, a(5)=1, a(6)=-4
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since a(n) repeats every 6 terms, use modulo arithmetic. */
    static const seq_t pattern[6] = {
        (seq_t)1,  (seq_t)0,  (seq_t)-1,
        (seq_t)0,  (seq_t)1,  (seq_t)-4
    };
    /* n>=1, so n-1 is safe. Index into the pattern array. */
    idx_t idx = (n - 1) % 6;
    return pattern[idx];
}
