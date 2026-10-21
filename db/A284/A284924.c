/*
 * A284924  a(n) is the least n-digit multiple of 5^n using the decimal digits
 *          {5, 6, 7, 8, 9} exclusively.
 * https://oeis.org/A284924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 5. For n > 1: Let k = a(n‑1) / 5^(n‑1) (k is an integer).
 *            Let t = 2^(n‑1) (mod 5). Find the smallest digit d ∈ {5,6,7,8,9}
 *            such that (d * t + k) ≡ 0 (mod 5). Then a(n) = d * 10^(n‑1) +
 *            a(n‑1). This iteratively constructs the least n‑digit multiple
 *            of 5^n whose decimal digits are all in {5,...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1, but guard for safety */

    seq_t a_curr = (seq_t)5;          /* a(1) */

    if (n == 1) return a_curr;

    /* State for iterative construction */
    seq_t pow5   = (seq_t)5;   /* 5^(1)  – holds 5^(pos-1) at iteration pos */
    seq_t pow10  = (seq_t)10;  /* 10^(1) – holds 10^(pos-1) at iteration pos */
    int    pow2_mod5 = 1;      /* 2^(pos-1) mod 5 ; init for pos = 2 */

    for (idx_t pos = 2; pos <= n; ++pos)
    {
        /* Update 2^(pos-1) mod 5 */
        pow2_mod5 = (pow2_mod5 * 2) % 5;

        /* k = a_curr / 5^(pos-1) */
        seq_t k = a_curr / pow5;
        int k_mod5 = (int)(k % 5);

        /* Find smallest admissible digit d */
        int d;
        for (d = 5; d <= 9; ++d)
        {
            if ( ((d * pow2_mod5 + k_mod5) % 5) == 0 )
                break;
        }

        /* Compute a(next) = d * 10^(pos-1) + a_curr */
        seq_t new_a = pow10 * (seq_t)d + a_curr;

        /* Prepare for next iteration */
        a_curr = new_a;
        pow5   = pow5 * (seq_t)5;
        pow10  = pow10 * (seq_t)10;
    }

    return a_curr;
}
