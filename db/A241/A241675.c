/*
 * A241675  a(n) = |{0 < k < n/2: k is a Fibonacci number with x^2 == k (mod
 *          n) for no integer x}|.
 * https://oeis.org/A241675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of Fibonacci numbers k with 0 < k < n/2 * for which
 *            the congruence x^2 ≡ k (mod n) has no integer solution
 * Range:     u8 n <= 356, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 4)
        return 0;

    seq_t count = 0;
    seq_t fib_prev = 1;   /* F(1) */
    seq_t fib_curr = 1;   /* F(2) */
    seq_t prev_fib = 0;   /* to skip the duplicate 1 */

    /* Iterate over Fibonacci numbers 2*k < n (i.e. k < n/2 real). */
    while ( ((idx_t)fib_prev) * 2 < n )
    {
        if (fib_prev != prev_fib)               /* ignore duplicate 1 */
        {
            bool has_root = false;
            for (idx_t x = 0; x < n; ++x)
            {
                idx_t sq = ((idx_t)x * x) % n;
                if (sq == (idx_t)fib_prev)
                {
                    has_root = true;
                    break;
                }
            }
            if (!has_root)
                ++count;
        }

        /* Advance to the next Fibonacci number: F_{i+1} = F_i + F_{i-1}. */
        seq_t next = fib_prev + fib_curr;
        prev_fib = fib_prev;
        fib_prev = fib_curr;
        fib_curr = next;
    }

    return count;
}
