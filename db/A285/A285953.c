/*
 * A285953  Positions of 0 in A285952; complement of A285954.
 * https://oeis.org/A285953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 1 + parity(popcount(2*n-1))
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base value 3*n-1.  All arithmetic is done in seq_t to avoid overflow. */
    seq_t val = (seq_t)3 * (seq_t)n - (seq_t)1;

    /* Compute parity of popcount(2*n-1) using 64‑bit arithmetic.
       popcount(2*n-1) parity equals XOR of all bits of the 64‑bit low
       part and the most significant bit after subtraction. */
    uint64_t low_part = ((uint64_t)n << 1) - 1ULL;
    uint8_t parity_low = (uint8_t)__builtin_parityll(low_part);

    /* The high bit after the subtraction.  For n == 2^63 the high bit
       is lost (borrow from the low part), otherwise it equals the
       original MSB of n. */
    uint8_t parity_high;
    if (n == (1ULL << 63)) {
        parity_high = 0;
    } else {
        parity_high = (uint8_t)(n >> 63);
    }

    /* Final parity: parity(low_part XOR high_bit). */
    uint8_t parity = parity_low ^ parity_high;

    return val + (seq_t)parity;
}
