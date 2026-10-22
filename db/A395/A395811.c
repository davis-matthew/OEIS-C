/*
 * A395811  Number of Fibonacci divisors of n that are at most sqrt(n).
 * https://oeis.org/A395811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = number of Fibonacci divisors of n that are ≤ √n
 * Range:     u8 n <= 63, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Count Fibonacci divisors of n up to sqrt(n).  */
static seq_t count_fib_divisors(idx_t n) {
    /* Compute integer floor of sqrt(n). */
    seq_t sqrt_n = floor_sqrt((seq_t)n);
    idx_t limit = (idx_t)sqrt_n;

    /* Count the divisor 1 which is always present for n ≥ 1. */
    seq_t count = 0;
    if (limit >= 1) {
        count++;
    }

    /* Generate Fibonacci numbers.  Start from 2 (the first unique value
       after the initial 1).  */
    seq_t a = 1;   /* previous Fibonacci */
    seq_t b = 1;   /* current Fibonacci */
    seq_t f = a + b;  /* next Fibonacci: 2 */

    seq_t n_seq = (seq_t)n;   /* local copy for modulo */

    while (f <= (seq_t)limit) {
        if (n_seq % f == 0) {
            count++;
        }
        /* Advance the sequence: a,b <- b,f and f <- a+b. */
        a = b;
        b = f;
        f = a + b;
    }

    return count;
}

/* External function matching the required signature. */
seq_t a(idx_t n) {
    return count_fib_divisors(n);
}
