/*
 * A366488  a(n) = (n^11 - n)/11.
 * https://oeis.org/A366488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^11 - n)/11
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 70, u128 n <= 3958,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Base for the internal big integer representation (fits in 32 bits) */
#define BIGBASE 1000000000U

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1)
        return (seq_t)0;

    /* convert n to 64‑bit for intermediate computations */
    uint64_t factor = (uint64_t)n;
    /* internal big integer: digits in base BIGBASE, little endian */
    uint32_t digits[32];
    size_t len = 0;

    /* initialise with n */
    uint64_t tmp = factor;
    do {
        digits[len++] = (uint32_t)(tmp % BIGBASE);
        tmp /= BIGBASE;
    } while (tmp);

    /* multiply by n ten more times to obtain n^11 */
    for (int i = 1; i < 11; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * factor + carry;
            digits[j]   = (uint32_t)(prod % BIGBASE);
            carry       = prod / BIGBASE;
        }
        if (carry) {
            digits[len++] = (uint32_t)carry;
        }
    }

    /* subtract n (subtract from the least significant digit; n < BIGBASE) */
    uint64_t sub = factor;           /* sub < BIGBASE */
    size_t pos = 0;
    if (digits[0] >= sub) {          /* no borrow */
        digits[0] -= (uint32_t)sub;
    } else {                         /* borrow 1 */
        digits[0] = (uint32_t)(digits[0] + BIGBASE - sub);
        pos = 1;
        while (pos < len && digits[pos] == 0) {
            digits[pos] = BIGBASE - 1;
            pos++;
        }
        if (pos < len) {
            digits[pos]--;
        }
    }

    /* remove leading zeros */
    while (len > 1 && digits[len - 1] == 0)
        --len;

    /* divide by 11 (big integer division by small integer) */
    uint64_t rem = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        uint64_t cur = rem * BIGBASE + digits[i];
        digits[i] = (uint32_t)(cur / 11);
        rem      = cur % 11;
    }
    /* rem must be zero because the expression is always divisible by 11 */
    (void)rem; /* silence unused warning in some compilers */

    /* remove leading zeros after division */
    while (len > 1 && digits[len - 1] == 0)
        --len;

    /* convert the resulting big integer to seq_t */
    seq_t result = 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        result = result * (seq_t)BIGBASE + (seq_t)digits[i];
    }
    return result;
}
