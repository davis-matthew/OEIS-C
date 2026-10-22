/*
 * A329002  a(n) is the smallest d such that the sum of digits of 2^d + n is
 *          equal to d, or -1 if no such d exists.
 * https://oeis.org/A329002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smallest d such that digit_sum(2^d + n) == d, or -1
 * Range:     u8 n <= 30, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define BASE 1000000000u      /* Decimal base for limb representation 1e9 */
#define LIMBS_MAX 1200        /* Enough for d up to ~20000 (≈6000 decimal digits) */
#define MAX_D 50000           /* Safety bound for search depth */

seq_t a(idx_t n)
{
    /* Representation of 2^d in base BASE, little‑endian limbs */
    uint32_t pow_limbs[LIMBS_MAX];
    size_t pow_len = 1;
    pow_limbs[0] = 1;                     /* 2^0 */

    /* Temporary buffer for 2^d + n */
    uint32_t tmp_limbs[LIMBS_MAX];
    size_t tmp_len;

    for (idx_t d = 1;; d++) {
        /* Double pow_limbs to obtain 2^d from 2^(d-1) */
        uint64_t carry = 0;
        for (size_t i = 0; i < pow_len; i++) {
            uint64_t val = (uint64_t)pow_limbs[i] * 2 + carry;
            pow_limbs[i] = (uint32_t)(val % BASE);
            carry = val / BASE;
        }
        if (carry > 0) {
            if (pow_len >= LIMBS_MAX)
                return (seq_t)-1;
            pow_limbs[pow_len++] = (uint32_t)carry;
        }

        /* Copy power into tmp buffer */
        tmp_len = pow_len;
        for (size_t i = 0; i < pow_len; i++)
            tmp_limbs[i] = pow_limbs[i];

        /* Add small integer n to tmp_limbs */
        uint64_t c = n;
        size_t i = 0;
        while (c > 0 && i < tmp_len) {
            uint64_t sum = (uint64_t)tmp_limbs[i] + c;
            tmp_limbs[i] = (uint32_t)(sum % BASE);
            c = sum / BASE;
            ++i;
        }
        while (c > 0) {
            if (tmp_len >= LIMBS_MAX)
                return (seq_t)-1;
            tmp_limbs[tmp_len++] = (uint32_t)(c % BASE);
            c /= BASE;
        }

        /* Compute digit sum of tmp_limbs */
        uint32_t digit_sum = 0;
        for (size_t j = 0; j < tmp_len; j++) {
            uint32_t limb = tmp_limbs[j];
            while (limb > 0) {
                digit_sum += limb % 10;
                limb /= 10;
            }
        }

        if (digit_sum == (uint32_t)d)
            return (seq_t)d;

        /* Failure bound to prevent infinite search */
        if (d > MAX_D)
            return (seq_t)-1;
    }
}
