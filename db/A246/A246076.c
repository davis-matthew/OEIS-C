/*
 * A246076  Paradigm shift sequence for the (-2,5) production scheme with
 *          replacement.
 * https://oeis.org/A246076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * a(n-8) (for n >= 25), with initial values a(1) = 1,
 *            a(2) = 2, ..., a(24) = 33 as given in the OEIS entry
 * Range:     u8 n <= 47, u16 n <= 111, u32 n <= 239, u64 n <= 495,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 24 values of the sequence */
    static const seq_t init[24] = {
        1ULL, 2ULL, 3ULL, 4ULL, 5ULL, 6ULL, 7ULL, 8ULL,
        9ULL, 10ULL, 11ULL, 12ULL, 13ULL, 14ULL, 15ULL, 16ULL,
        18ULL, 20ULL, 22ULL, 24ULL, 26ULL, 28ULL, 30ULL, 33ULL
    };

    if (n <= 24)
        return init[n - 1];

    idx_t k = n - 24;       /* number of steps beyond the base block */
    idx_t t = (k + 7) / 8;  /* number of times the recurrence is applied   */
    seq_t factor = 1;

    /* Compute 2^t in a loop to avoid overflow in a 64‑bit build when
       the value still fits into seq_t. */
    while (t--)
        factor *= 2;

    /* Find the base index after shifting by 8*t steps.   */
    idx_t base_idx = n - 8 * ((n - 24 + 7) / 8); /* n - 8*t */

    return factor * init[base_idx - 1];
}
