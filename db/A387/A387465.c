/*
 * A387465  Irregular triangle T(n,k), n >= 0, 0 <= k < 2^(n-1), where T(n,k)
 *          = Product_{j=0..n-1} prime(n-j)^((j+1)*d_j), where d_j is the bit
 *          with digit weight 2^j in the binary expansion of 2^(n-1)+k.
 * https://oeis.org/A387465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = ∏_{j=0}^{r-1, bit_j(n)=1} prime(r-j)^(j+1) where r =
 *            ⌊log₂ n⌋ + 1 (for n > 0), and a(0) = 1. This follows directly
 *            from the triangle definition: T(r,k) = ∏_{j=0}^{r-1}
 *            prime(r-j)^{(j+1)*d_j} with d_j the j‑th binary digit of n =
 *            2^(r-1)+k
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 46, u64 n <= 188, u128 n <= 766,
 *            big any n
 * Helpers:   big_pow, floor_log2_uint64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* r = floor(log2(n)) + 1   (since n > 0) */
    idx_t r = floor_log2_uint64(n) + 1;

    seq_t result = (seq_t)1;

    for (idx_t j = 0; j < r; ++j)
    {
        if ((n >> j) & (idx_t)1)          /* bit j of n is set? */
        {
            idx_t prime_idx = r - j;       /* prime(r-j) */
            seq_t prime_val = (seq_t)get_prime(prime_idx);  /* kth prime (k≥1) */

            /* exponent = j+1 (small, fits in seq_t) */
            seq_t pow_val = big_pow(prime_val, (seq_t)(j + 1));

            /* multiply into the running product */
            result *= pow_val;               /* works for builtin and big‑integer types */
        }
    }

    return result;
}
