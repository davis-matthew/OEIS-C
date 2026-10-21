/*
 * A275981  Decimal expansion of 2^19937 - 1, the 24th Mersenne prime
 *          A000668(24).
 * https://oeis.org/A275981
 *
 * Offset:    6002    (a(6002) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   decimal expansion of 2^19937 - 1, the 24th Mersenne prime
 *            Sequence a(n) gives the n‑th decimal digit of this 6005‑digit
 *            number. Offset: a(6002) is the most‑significant digit (the first
 *            listed term)
 * Range:     u8 n <= 9845, u16 n <= 9845, u32 n <= 9845, u64 n <= 9845,
 *            u128 n <= 9845, big any n
 * Helpers:   append_uint32_to_buffer
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "../utils/append_uint32_to_buffer.h"

/* Big integer base 1,000,000,000; enough for 6005 decimal digits
   6005 / 9 ≈ 667, so allocate a little more.                     */
#define BIG_BASE 1000000000U
#define BIG_CAP  700U      /* > 667 */

static int initialized = 0;
static char digits[6005];
static size_t digits_len = 0;

static void init_digits(void)
{
    /* 1. Compute 2^19937 using repeated doubling */
    static uint32_t big[BIG_CAP];
    size_t big_len = 1;
    big[0] = 1;

    /* Multiply by 2, 19937 times */
    for (int step = 0; step < 19937; ++step) {
        uint64_t carry = 0;
        for (size_t i = 0; i < big_len; ++i) {
            uint64_t prod = (uint64_t)big[i] * 2 + carry;
            big[i] = (uint32_t)(prod % BIG_BASE);
            carry   = prod / BIG_BASE;
        }
        if (carry > 0) {
            big[big_len++] = (uint32_t)carry;
        }
    }

    /* 2. Subtract 1 (big integer minus 1) */
    size_t i = 0;
    while (i < big_len && big[i] == 0) {
        big[i] = BIG_BASE - 1;
        ++i;
    }
    if (i < big_len) {
        big[i] -= 1;
    }

    /* 3. Convert big integer to decimal string, most significant first */
    char buf[6005];
    size_t pos = 0;
    /* Most significant word: no leading zeros */
    append_uint32_to_buffer(big[big_len - 1], buf, &pos, 0);
    /* Remaining words: exactly 9 digits each, padded with zeros */
    for (int j = (int)big_len - 2; j >= 0; --j) {
        append_uint32_to_buffer(big[j], buf, &pos, /*width*/9);
    }
    /* Copy to global digits; convert each char to digit value */
    digits_len = pos;
    for (size_t k = 0; k < digits_len; ++k) {
        digits[k] = buf[k];
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_digits();
    }

    /* a(6002) is the first digit stored */
    if (n < 6002) {
        /* Undefined below the offset; return 0 */
        return (seq_t)0;
    }

    idx_t offset = n - 6002;
    if ((size_t)offset >= digits_len) {
        /* Out of bounds – not expected */
        return (seq_t)0;
    }

    return (seq_t)(digits[(size_t)offset] - '0');
}
