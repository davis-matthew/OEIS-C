/*
 * A297251  Numbers whose base-3 digits have greater up-variation than
 *          down-variation; see Comments.
 * https://oeis.org/A297251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑3 representation
 *            starts with 1 and ends with 2
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Find the length L of the base‑3 representation (>=2) that contains
       the nth number.  For length L, there are 3^(L‑2) such numbers.  */
    idx_t L = 2;
    idx_t cumulative = 0;         /* numbers of all shorter lengths */
    idx_t power = 1;              /* 3^(L‑2) */
    while (cumulative + power < n) {
        cumulative += power;
        power *= 3;
        ++L;
    }

    /* Zero‑based index of the desired number within the block of length L */
    idx_t index_mid = n - cumulative - 1;

    /* Length of the middle part (digits between the leading 1 and the
       trailing 2).  Can be 0 for length 2. */
    idx_t mid_len = (L > 2) ? (L - 2) : 0;

    /* Extract the base‑3 digits of ``index_mid`` (little‑endian). */
    uint8_t mid_digits[128] = {0};
    idx_t temp = index_mid;
    for (idx_t i = 0; i < mid_len; ++i) {
        mid_digits[i] = (uint8_t)(temp % 3);
        temp /= 3;
    }

    /* Assemble the final number from the digits: 1, middle digits,
       then 2.  The arithmetic uses seq_t, which may be an
       arbitrary‑precision type when needed. */
    seq_t result = 1;
    if (mid_len > 0) {
        for (int i = (int)mid_len - 1; i >= 0; --i)
            result = result * 3 + mid_digits[i];
    }
    result = result * 3 + 2;  /* trailing 2 */

    return result;
}
