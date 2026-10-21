/*
 * A293436  a(n) is the sum of the proper divisors of n that are Fibonacci
 *          numbers (A000045).
 * https://oeis.org/A293436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of proper divisors of n that are Fibonacci numbers
 *            (A293436). This implementation iterates over the Fibonacci
 *            sequence up to n‑1 and sums the ones that divide n. Uniqueness
 *            is maintained by generating the Fibonacci numbers 1, 2, 3, 5,
 *            ... (skipping the duplicate initial 1). The routine...
 * Range:     u8 n <= 753, u16 n <= 9765, u32 n <= 9765, u64 n <= 9765,
 *            u128 n <= 9765, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 1 has no proper divisors */
    if (n <= 1)
        return 0;

    seq_t sum = 0;

    /* 1 is always a Fibonacci proper divisor for n > 1 */
    sum += (seq_t)1;

    /* Initial Fibonacci numbers: f_prev = 1, f_curr = 2 (unique sequence) */
    idx_t f_prev = 1;
    idx_t f_curr = 2;

    /* Check Fibonacci divisors up to n‑1 */
    while (f_curr <= n - 1)
    {
        if (n % f_curr == 0)
            sum += (seq_t)f_curr;

        /* Safe Fibonacci generation: avoid 64‑bit overflow
           (UINT64_MAX is guaranteed to be defined in <stdint.h>) */
        if (f_prev > (UINT64_MAX - f_curr))
            break;                /* next fib would overflow 64 bits */

        idx_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    return sum;
}
