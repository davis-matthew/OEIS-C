/*
 * A250874  Number of (n+1) X (5+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A250874. Computes values via the recurrence
   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4).
   Initial values: a(1)=2306, a(2)=11644, a(3)=55386, a(4)=250964. */

#include <stdbool.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, return 0 */

    /* initial terms */
    const seq_t init[4] = {2306, 11644, 55386, 250964};

    if (n <= 4) return init[n - 1];

    seq_t prev4 = init[0];   /* a(n-4) */
    seq_t prev3 = init[1];   /* a(n-3) */
    seq_t prev2 = init[2];   /* a(n-2) */
    seq_t prev1 = init[3];   /* a(n-1) */
    seq_t current = 0;

    for (idx_t i = 5; i <= n; ++i) {
        /* compute next term using the recurrence */
        current = mul_small(prev1, 10)
                  - mul_small(prev2, 35)
                  + mul_small(prev3, 50)
                  - mul_small(prev4, 24);

        /* shift the window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
