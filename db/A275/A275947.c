/*
 * A275947  Number of distinct slopes with multiple nonzero digits in
 *          factorial base representation of n: a(n) = A056170(A275734(n)).
 *          (See comments for more exact definition).
 * https://oeis.org/A275947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct values of (position − digit) that
 *            occur more than once in the factorial‑base representation of n
 * Range:     u8 n <= 9632, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Convert the index to a mutable variable */
    idx_t temp = n;

    /* Factorial base digits are stored as (pos, digit) pairs with
       pos counting from 1 on the right (least‑significant digit). */
    uint32_t pos = 1;  /* position index */
    uint32_t counts[64];
    memset(counts, 0, sizeof(counts));

    while (temp > 0) {
        /* In factorial base, the digit at position 'pos' is
           temp mod (pos + 1), and then we divide temp by (pos + 1). */
        uint64_t digit = temp % (pos + 1);
        temp /= (pos + 1);

        if (digit > 0) {
            /* Compute the "slope" value: position minus digit.
               Both are small (pos ≤ 20 for 64‑bit n), so the result fits in
               a 32‑bit integer. */
            uint32_t val = pos - (uint32_t)digit;
            counts[val]++;
        }
        pos++;
    }

    /* Count how many distinct values have multiplicity > 1. */
    seq_t result = 0;
    for (int i = 0; i < (int)sizeof(counts)/sizeof(counts[0]); ++i) {
        if (counts[i] > 1) {
            result++;
        }
    }

    return result;
}
