/*
 * A354774  For terms of A354169 that are the sum of two distinct powers of 2,
 *          the exponent of the larger power of 2.
 * https://oeis.org/A354774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+2, except * if n = 3*2^(k-1)-3 (k>=2) → a(n) = (n+1)/2,
 *            * except a(3)=a(9)=4 and a(21)=5 * if n = 2^(k+1)-3 (k>=1) →
 *            a(n) = (n+1)/2, * except a(5)=a(13)=6 and a(29)=7 * if n =
 *            3*2^(k-1)-2 (k>=2) → a(n) = n+1 * if n = 2^(k+1)-2 (k>=1) → a(n)
 *            = n+1
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Pattern I: n = 3*2^(k-1)-3  →  n+3 = 3*2^(k-1) */
    if ((n + 3) % 3 == 0) {
        seq_t p = ((seq_t)(n + 3)) / 3;
        if (is_power_of_two(p) && p >= 2) {
            if (n == 3 || n == 9) return 4;
            if (n == 21)          return 5;
            return ((seq_t)n + 1) / 2;
        }
    }

    /* Pattern II: n = 2^(k+1)-3  →  n+3 = 2^(k+1) */
    if (n + 3 >= 4 && is_power_of_two((seq_t)(n + 3))) {
        if (n == 5 || n == 13) return 6;
        if (n == 29)          return 7;
        return ((seq_t)n + 1) / 2;
    }

    /* Pattern III: n = 3*2^(k-1)-2  →  n+2 = 3*2^(k-1) */
    if ((n + 2) % 3 == 0) {
        seq_t p = ((seq_t)(n + 2)) / 3;
        if (is_power_of_two(p) && p >= 2)
            return ((seq_t)n) + 1;
    }

    /* Pattern IV: n = 2^(k+1)-2  →  n+2 = 2^(k+1) */
    if (n + 2 >= 4 && is_power_of_two((seq_t)(n + 2)))
        return ((seq_t)n) + 1;

    /* Default rule */
    return ((seq_t)n) + 2;
}
