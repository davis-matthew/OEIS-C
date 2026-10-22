/*
 * A356994  a(n) = n - b(b(b(n))), where b(n) = A356988(n).
 * https://oeis.org/A356994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - b(b(b(n))) where b(n)=A356988(n)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented using the block‑based description from the OEIS entry. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases, matching the OEIS offset */
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Pre‑compute Fibonacci numbers in an array of 64‑bit unsinged ints.
       The array is indexed from 1, with fib[1] = fib[2] = 1.  */
    static unsigned long long fib[64];
    static int fib_cnt = 0;
    if (!fib_cnt) {
        fib[1] = 1;
        fib[2] = 1;
        fib_cnt = 2;
        while (fib_cnt < 63) {
            unsigned long long next = fib[fib_cnt] + fib[fib_cnt - 1];
            if (next < fib[fib_cnt]) break;      /* overflow guard */
            fib[++fib_cnt] = next;
        }
    }

    /* Iterate over the blocks defined by k >= 2.  For each k we
       have a plateau [F(k+2), 3*F(k)] with value 2*F(k), followed by
       a ramp [3*F(k)+1, 3*F(k)+2*F(k-1)] where the value increments
       by 1 each step. */
    for (int k = 2; ; ++k) {
        unsigned long long plat_end = fib[k] * 3ULL;
        if (n <= plat_end) {
            return (seq_t)(2ULL * fib[k]);                 /* plateau */
        }
        unsigned long long ramp_end = plat_end + 2ULL * fib[k - 1];
        if (n <= ramp_end) {
            return (seq_t)(2ULL * fib[k] + (unsigned long long)(n - plat_end)); /* ramp */
        }
    }
}
