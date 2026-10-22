/*
 * A353058  Minimum number of iterations {add or subtract 1, or half if even}
 *          needed to reach 1, starting from n.
 * https://oeis.org/A353058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: computes A353058(n) – minimal number of operations
   (add 1, subtract 1, or divide by 2 if even) required to reduce n to 1.
   Uses the optimal greedy strategy proven for the integer‑replacement problem. */

seq_t a(idx_t n) {
    /* a(1) = 0 by definition */
    if (n <= 1) return (seq_t)0;

    seq_t x = (seq_t)n;          /* current value */
    seq_t steps = 0;             /* count of operations */

    while (x > (seq_t)1) {
        if ((x & (seq_t)1) == 0) {          /* x even: divide by 2 */
            x >>= 1;
        } else {                            /* x odd */
            /* For odd numbers, choose +/-1 to produce a value divisible by 4
               unless x is 3, where subtracting 1 is optimal. */
            if (x == (seq_t)3 || ((x & (seq_t)3) == 1)) {
                x -= 1;                     /* go to x-1 */
            } else {
                x += 1;                     /* go to x+1 */
            }
        }
        ++steps;
    }
    return steps;
}
