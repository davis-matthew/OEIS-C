/*
 * A284560  a(n) = n minus (LCM of run lengths in binary representation of n)
 *          = n - A284559(n).
 * https://oeis.org/A284560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (LCM of run lengths in binary representation of n)
 * Range:     u8 n <= 260, u16 n <= 9166, u32 n <= 9166, u64 n <= 9166,
 *            u128 n <= 9166, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n >= 1 (action for n == 0 handled trivially) */
    idx_t temp = n;
    seq_t lcm_val = 0;          /* accumulates the LCM of run lengths    */
    idx_t current_bit = temp & 1ULL;   /* bit value of current run          */
    seq_t run_len = 0;          /* length of the current run          */

    /* Traverse bits from LSB upward, building runs */
    while (temp > 0) {
        if ((temp & 1ULL) == current_bit) {
            run_len++;                 /* same run continues                */
        } else {
            /* finalize previous run */
            if (lcm_val == 0) {
                lcm_val = run_len;     /* first run encountered             */
            } else {
                seq_t g = gcd(lcm_val, run_len);
                lcm_val = (lcm_val / g) * run_len;
            }
            /* start new run */
            current_bit = temp & 1ULL;
            run_len = 1;
        }
        temp >>= 1;
    }

    /* finalize the last run */
    if (lcm_val == 0) {
        lcm_val = run_len;
    } else {
        seq_t g = gcd(lcm_val, run_len);
        lcm_val = (lcm_val / g) * run_len;
    }

    /* result */
    return (seq_t)n - lcm_val;
}
