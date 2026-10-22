/*
 * A392112  Numbers k such that 5*k - (greatest prime < 5*k) > (least prime >
 *          5*k) - 5*k.
 * https://oeis.org/A392112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392112: Numbers k such that  
 * 5*k - (largest prime < 5*k) > (smallest prime > 5*k) - 5*k.
 * The function computes a(n), the n-th such integer.
 * (Sequence starts at a(1) = 2.)
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;     /* number of found terms */
    seq_t k = 1;         /* current candidate k */

    while (count < n) {
        /* Compute 5*k */
        seq_t N = (seq_t)5 * k;

        /* Find largest prime < N */
        seq_t p_low = 0;
        seq_t t = N - 1;
        while (t > 0) {
            if (is_prime((idx_t)t)) {
                p_low = t;
                break;
            }
            --t;
        }

        /* Find smallest prime > N */
        seq_t p_up = N + 1;
        while (!is_prime((idx_t)p_up)) {
            ++p_up;
        }

        /* Test the inequality */
        if ((N - p_low) > (p_up - N)) {
            ++count;
            if (count == n) {
                return k;
            }
        }

        ++k;
    }

    /* Should never reach here since an infinite number of solutions exist */
    return 0;
}
