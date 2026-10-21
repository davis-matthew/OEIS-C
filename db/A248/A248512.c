/*
 * A248512  Sequence based on repeating in turn the subtract, add and multiply
 *          operations on the previous 2 terms, starting with 0, 1.
 * https://oeis.org/A248512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 16, i16 n <= 19, i32 n <= 22, i64 n <= 25, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0 a(2)=1
   For n>2:
   if n%3==0: a(n)=a(n-1)-a(n-2)
   if n%3==1: a(n)=a(n-1)+a(n-2)
   if n%3==2: a(n)=a(n-1)*a(n-2) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    seq_t prev2 = (seq_t)0;   /* a(1) */
    seq_t prev1 = (seq_t)1;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr;
        idx_t r = i % 3;
        if (r == 0) {
            curr = prev1 - prev2;
        } else if (r == 1) {
            curr = prev1 + prev2;
        } else {            /* r == 2 */
            curr = prev1 * prev2;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
