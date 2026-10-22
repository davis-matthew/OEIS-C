/*
 * A398423  Largest integer of the form r^k whose weighted base-n digit sum is
 *          r and whose number of base-n digits is k.
 * https://oeis.org/A398423
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, base
 * Formula:   a(n) = max r^k such that (1) W_n(r^k)=r (2) r^k has exactly k
 *            digits in base n. The function searches all r from 1 to n-1 and
 *            iterates k while the digit count of r^k remains ≥ k. The
 *            weighted sum is computed by repeated division
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* lower offsets are irrelevant */
        return 0;

    seq_t best = 0;

    for (idx_t r = 1; r < n; ++r)
    {
        seq_t r_val   = (seq_t)r;
        seq_t power   = r_val;          /* r^k, start with k=1 */

        for (idx_t k = 1; ; ++k)
        {
            /* compute weighted digit sum and number of digits */
            seq_t   temp     = power;
            seq_t   weighted = 0;
            seq_t   weight   = 1;
            idx_t   len      = 0;

            while (temp != 0)
            {
                seq_t digit = temp % (seq_t)n;
                weighted += digit * weight;
                temp     /= (seq_t)n;
                ++len;
                ++weight;
            }

            if (len == k && weighted == r_val)
            {
                if (power > best)
                    best = power;
            }

            /* if the digit count has fallen below k, no larger k will work */
            if (len < k)
                break;

            /* prepare next power; stop if multiplication overflows 128‑bit */
            seq_t next_power = power * r_val;
            if (next_power < power)    /* wrap‑around indicates overflow */
                break;

            power = next_power;
        }
    }

    return best;
}
