/*
 * A395229  a(n) = number of 4-tuples (w, x, y, z) such that w*x + y*z = n,
 *          where w, x, y, z are distinct Fibonacci numbers satisfying w*x <
 *          y*z.
 * https://oeis.org/A395229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Count of 4‑tuples (w,x,y,z) of distinct Fibonacci numbers
 *            satisfying w*x + y*z = n and w*x < y*z. Computation is by
 *            exhaustive search over all Fibonacci numbers ≤ n
 * Range:     u8 n <= 40, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Generate the Fibonacci numbers (unique values) that are ≤ n.  */
    const int MAX_FIB = 2000;                 /* ample for all practical n */
    seq_t fib[MAX_FIB];
    int fibcnt = 0;
    seq_t limit = (seq_t)n;

    seq_t a_val = (seq_t)1;
    seq_t b_val = (seq_t)2;
    while (a_val <= limit && fibcnt < MAX_FIB) {
        fib[fibcnt++] = a_val;
        seq_t next = a_val + b_val;
        a_val = b_val;
        b_val = next;
    }
    if (fibcnt == 0) return 0;                /* n < 1 → no tuples */

    seq_t half = limit / 2;                   /* ensures w*x < y*z */
    seq_t count = 0;

    for (int i = 0; i < fibcnt; ++i) {
        for (int j = i + 1; j < fibcnt; ++j) {
            seq_t p1 = fib[i] * fib[j];
            if (p1 > half) break;             /* further j give larger p1 */
            seq_t target = limit - p1;         /* required product for y*z */

            for (int k = 0; k < fibcnt; ++k) {
                if (k == i || k == j) continue;
                for (int l = k + 1; l < fibcnt; ++l) {
                    if (l == i || l == j) continue;
                    if (fib[k] * fib[l] == target) count++;
                }
            }
        }
    }
    return count;
}
