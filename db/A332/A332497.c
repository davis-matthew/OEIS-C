/*
 * A332497  a(n) = x(w+1) where x(0) = 0 and x(k+1) = 2^(k+1)-1-x(k) (resp.
 *          x(k)) when d_k = 1 (resp. d_k <> 1) and Sum_{k=0..w} d_k*3^k is
 *          the ternary representation of n. Sequence A332498 gives
 *          corresponding y's.
 * https://oeis.org/A332497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by the recurrence * x(0) = 0 * for k =
 *            0,1,2,... * if the k‑th ternary digit of n is 1 * x(k+1) =
 *            2^(k+1) - 1 - x(k) * else * x(k+1) = x(k) * a(n) = x(w+1) where
 *            w is the index of the highest non‑zero ternary digit. *
 *            Equivalent: iterate over the ternary digits from least
 *            signifi...
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x     = 0;      /* current value of x(k)          */
    seq_t pow2  = 2;      /* 2^(k+1) for current k (k=0)    */
    idx_t n_temp = n;     /* working copy of n              */

    /* Process at least one digit (for n = 0) and continue while
     * there are more digits to process. */
    do
    {
        uint64_t digit = n_temp % 3;   /* less than 3, fits in 64‑bit */
        if (digit == 1)
        {
            x = (pow2 - 1) - x;
        }
        n_temp /= 3;
        pow2 <<= 1;
    } while (n_temp > 0);

    return x;
}
