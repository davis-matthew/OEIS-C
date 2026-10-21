/*
 * A290842  Numbers k such that the sum of digits of k^3 is 3^3 = 27.
 * https://oeis.org/A290842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   find the n-th integer k such that the sum of digits of k³ equals
 *            27
 * Range:     u8 n <= 32, u16 n <= 312, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple search; it scans k = 1,2,3,… and counts
   those whose cube has digit sum 27 until the n‑th such k is found. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the digit sum of a 128‑bit unsigned integer. */
static seq_t digit_sum_uint128(unsigned __int128 x)
{
    unsigned long long sum = 0;
    while (x != 0ULL) {
        sum += (unsigned)(x % 10);
        x /= 10;
    }
    return (seq_t)sum;   /* sum ≤ 108, fits in any seq_t */
}

/* Return the n‑th term of the sequence (a(1) = 27).  If n == 0,
   returns 0; the offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t found = 0;           /* how many valid k's have been seen */
    idx_t k = 0;               /* candidate k (idx_t to avoid overflow) */
    seq_t result = 0;

    while (found < n) {
        ++k;
        /* compute k³ using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 cube = (unsigned __int128)k * k * k;

        /* digit sum of the cube */
        if (digit_sum_uint128(cube) == 27) {
            ++found;
            result = (seq_t)k;   /* k fits into seq_t for the values that
                                     are tested in each build */
        }
    }
    return result;
}
