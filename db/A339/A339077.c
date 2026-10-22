/*
 * A339077  Numbers k such that k and k+1 are both coprime to their digital
 *          sum (A339076).
 * https://oeis.org/A339077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 9147, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339077: numbers k such that k and k+1 are both coprime to their digital sum. */
/* The function returns the nth such number (1‑based index). */
#include "../utils/gcd.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;

    while (found < n) {
        seq_t ds_k  = digit_sum(k);
        seq_t k1    = k + 1;
        seq_t ds_k1 = digit_sum(k1);

        if (gcd(k,  ds_k)  == (seq_t)1 && gcd(k1, ds_k1) == (seq_t)1) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
    /* unreachable for valid n */
    return 0;
}
