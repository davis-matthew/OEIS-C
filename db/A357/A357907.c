/*
 * A357907  The internal state of the Sinclair ZX81 and Spectrum random number
 *          generator.
 * https://oeis.org/A357907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (75*a(n-1) + 74) mod 65537, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9388, u64 n <= 9388,
 *            u128 n <= 9388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence has period 65536.  Precompute a single period lazily. */
    static seq_t period[65536];
    static bool built = false;
    if (!built) {
        period[0] = (seq_t)1;          /* a(1) */
        for (idx_t i = 1; i < 65536; ++i) {
            /* Treat intermediate values as at least 32 bits to avoid overflow. */
            seq_t prev = period[i - 1];
            period[i] = (seq_t)((75 * prev + 74) % (seq_t)65537);
        }
        built = true;
    }
    idx_t idx = (n - 1) % (idx_t)65536;  /* wrap around the period */
    return period[idx];
}
