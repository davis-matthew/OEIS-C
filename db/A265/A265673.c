/*
 * A265673  Area A under the trajectory of each odd number in the "3x+1"
 *          problem.
 * https://oeis.org/A265673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = area under the 3x+1 trajectory of the nth odd integer
 *            (a(1) is defined as 0). The area is a(n) = (x+1)/2 +
 *            Σ_{k=1}^{r-1} x_k where x = 2*n-1, x_0 = x, x_{k+1} = T(x_k) (T
 *            is the Collatz map), and r is the number of steps needed to
 *            reach 1
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: first odd number 1 is defined as 0 */
    if (n == 1) return 0;

    /* Starting odd integer */
    seq_t x = (seq_t)(2 * n - 1);
    seq_t cur = x;
    seq_t sum = 0;            /* This will accumulate the sum of all
                                 intermediate values (excluding start
                                 and final 1). */

    while (cur != 1) {
        /* Compute next Collatz value */
        if ((cur & (seq_t)1) == 0) {      /* even */
            cur >>= 1;
        } else {                          /* odd */
            cur = cur * (seq_t)3 + 1;
        }
        /* Add the intermediate value if it is not the terminal 1 */
        if (cur != 1) {
            sum += cur;
        }
    }

    /* Compute area: (x+1)/2 + sum */
    seq_t area = (x + (seq_t)1) / 2 + sum;
    return area;
}
