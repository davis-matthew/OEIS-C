/*
 * A270124  Ratios of successive terms of A270121.
 * https://oeis.org/A270124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = A270121(n+1)/A270121(n) for n ≥ 1 A270121 is
 *            defined by x(1) = 7, x(2) = 112, x(n+1) = x(n)^2 * (1 + n *
 *            x(n)) / x(n-1) for n ≥ 1 We store the values of x[1] … x[10001]
 *            (enough for n ≤ 10000) and compute the ratios on demand. The
 *            array is large enough for all practical queries; m...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Keep the values of A270121.  We use 1‑based indexing; 
   x[0] is unused. x[1] = 7, x[2] = 112.  We allocate
   space for x[1] … x[10001] inclusive. */
static seq_t x[10002];
static idx_t x_last = 0;   /* largest index that has been initialized */

/* Initialize the first two terms of x and set x_last appropriately. */
static void init_x(void)
{
    if (x_last == 0) {
        x[1] = (seq_t)7;
        x[2] = (seq_t)112;
        x_last = 2;
    }
}

/* Ensure that x[i] is valid for all i ≤ target (target ≤ 10001). */
static void ensure_x(idx_t target)
{
    if (target <= x_last)
        return;
    init_x();
    for (idx_t i = x_last; i < target; ++i) {
        /* recurrence derived from x(n+1)*x(n-1) = x(n)^2 * (1 + n*x(n)) */
        seq_t yi   = (seq_t)i * x[i] + (seq_t)1;           /* 1 + i*x(i) */
        seq_t temp = x[i] * yi;                           /* x(i)^2 * (1 + i*x(i)) */
        seq_t left = x[i] * temp;                         /* x(i)^3 * (1 + i*x(i)) */
        x[i + 1] = left / x[i - 1];                       /* exact division */
    }
    x_last = target;
}

/* Public function returning the n-th term of A270124.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;          /* a(0) is defined as 2 */

    /* We need x[n+1] and x[n] to compute the ratio. */
    ensure_x(n + 1);
    return x[n + 1] / x[n];
}
