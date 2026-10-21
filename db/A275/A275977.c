/*
 * A275977  Decimal expansion of 2^9689 - 1, the 21st Mersenne prime
 *          A000668(21).
 * https://oeis.org/A275977
 *
 * Offset:    2917    (a(2917) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5833, u16 n <= 5833, u32 n <= 5833, u64 n <= 5833,
 *            u128 n <= 5833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal expansion of 2^9689 - 1.
   a(n) returns the n‑th decimal digit (1‑based) of that number.
   Index offset: a(2917) is the first digit of the expansion. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool initialized = false;
static uint8_t digits[4000];   /* enough to hold 2918 decimal digits */
static size_t digits_len = 0;

/* Compute 2^9689 - 1 and fill the digits array. */
static void init(void)
{
    /* big integer in base 1e9, little‑endian limbs */
    uint32_t limbs[400] = {0};
    size_t len = 1;
    limbs[0] = 1;                /* start with 1 */

    /* multiply by 2, 9689 times */
    for (int exp = 0; exp < 9689; ++exp) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t tmp = ((uint64_t)limbs[i] << 1) + carry; /* *2 + carry */
            limbs[i] = (uint32_t)(tmp % 1000000000u);
            carry   = tmp / 1000000000u;
        }
        if (carry) {
            limbs[len] = (uint32_t)carry;
            ++len;
        }
    }

    /* subtract 1 */
    size_t i = 0;
    while (i < len) {
        if (limbs[i] > 0) {
            --limbs[i];
            break;
        } else {
            limbs[i] = 1000000000u - 1;
            ++i;
        }
    }

    /* convert to decimal string (most significant digit first) */
    size_t pos = 0;

    /* most significant limb (no leading zeros) */
    {
        uint32_t x = limbs[len - 1];
        char buf[10];
        int blen = 0;
        do {
            buf[blen++] = (char)('0' + (x % 10));
            x /= 10;
        } while (x);
        for (int j = blen - 1; j >= 0; --j) {
            digits[pos++] = (uint8_t)(buf[j] - '0');
        }
    }

    /* remaining limbs, each contributes exactly 9 digits with leading zeros */
    for (int limb = (int)len - 2; limb >= 0; --limb) {
        uint32_t x = limbs[limb];
        /* extract 9 decimal digits in reverse order */
        uint8_t rev[9];
        for (int k = 0; k < 9; ++k) {
            rev[k] = (uint8_t)(x % 10);
            x /= 10;
        }
        /* output in correct order */
        for (int k = 8; k >= 0; --k) {
            digits[pos++] = rev[k];
        }
    }

    digits_len = pos;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    const idx_t offset = 2917;
    if (n < offset) {
        return (seq_t)0;   /* undefined for n < offset */
    }
    size_t pos = (size_t)(n - offset);
    if (pos >= digits_len) {
        return (seq_t)0;   /* out of bounds */
    }
    return (seq_t)digits[pos];
}
