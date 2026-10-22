/*
 * A345290  a(n) is obtained by replacing 2^k in binary expansion of n with
 *          Fibonacci(-k-2).
 * https://oeis.org/A345290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = Σ_{k≥0, bit k of n = 1} (-1)^{k+1} * Fib(k+2) (Fib is the
 *            standard Fibonacci sequence: Fib(0)=0, Fib(1)=1,
 *            Fib(n)=Fib(n-1)+Fib(n-2)). The replacement of 2^k in the binary
 *            expansion of n with Fib(-k-2) yields this closed form
 * Range:     i8 n <= 647, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool fib_inited = false;
static seq_t fib_arr[66]; /* fib_arr[i] stores Fib(i) for 0 ≤ i ≤ 65 */

/* Initialize the Fibonacci table up to Fib(65).  This is done once the
   first time a() is called.  */
static void init_fib(void)
{
    if (fib_inited) return;
    fib_arr[0] = 0;
    fib_arr[1] = 1;
    for (idx_t i = 2; i <= 65; ++i) {
        fib_arr[i] = fib_arr[i-1] + fib_arr[i-2];
    }
    fib_inited = true;
}

seq_t a(idx_t n)
{
    init_fib();

    seq_t sum = 0;
    idx_t k = 0;
    idx_t temp = n;
    while (temp != 0) {
        if ((temp & 1) != 0) {
            seq_t contrib = fib_arr[k+2];
            if ((k & 1) == 0) {
                contrib = -contrib; /* k even → negative contribution */
            }
            sum += contrib;
        }
        temp >>= 1;
        ++k;
    }
    return sum; /* a(0) correctly returns 0 (loop not entered) */
}
