/*
 * A371838  Decimal expansion of 666^666, the Legion's number of the first
 *          kind.
 * https://oeis.org/A371838
 *
 * Offset:    1881    (a(1881) is the first term)
 * Keywords:  nonn, easy, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3761, u16 n <= 3761, u32 n <= 3761, u64 n <= 3761,
 *            u128 n <= 3761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the (n‑1880)-th decimal digit of 666^666,
   for n ≥ 1881.  (That is, a(1881) is the first digit.) */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- Big‑integer helpers (base 1e9, little‑endian) ---- */
static void mul_small(uint32_t *num, size_t *len, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)num[i] * m + carry;
        num[i] = (uint32_t)(prod % 1000000000u);
        carry   = prod / 1000000000u;
    }
    while (carry) {
        num[(*len)++] = (uint32_t)carry;
        carry /= 1000000000u;
    }
}

/* ---- Compute the full decimal expansion of 666^666 ---- */
static void compute_digits(char *out, size_t *out_len)
{
    /* Temporary big integer, most significant word first */
    uint32_t num[256] = {0};
    size_t len = 1;
    num[0] = 1;

    const uint32_t base = 666;
    for (int i = 0; i < 666; ++i)
        mul_small(num, &len, base);

    /* Convert to decimal string */
    size_t pos = 0;

    /* Most significant word: no leading zeros */
    uint32_t val = num[len - 1];
    {
        char buf[10];
        int bl = 0;
        if (val == 0) {
            buf[bl++] = '0';
        } else {
            while (val) {
                buf[bl++] = '0' + (val % 10);
                val /= 10;
            }
        }
        for (int i = bl - 1; i >= 0; --i)
            out[pos++] = buf[i];
    }

    /* Remaining words: pad to 9 digits each */
    for (size_t w = len - 1; w > 0; --w) {
        uint32_t v = num[w - 1];
        for (int i = 0; i < 9; ++i) {
            char c = '0' + (v % 10);
            out[pos + 8 - i] = c;
            v /= 10;
        }
        pos += 9;
    }

    *out_len = pos;
}

/* ---- Main sequence function ---- */
static bool initialized = false;
static char digits[2000];
static size_t digits_len;

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_digits(digits, &digits_len);
        initialized = true;
    }

    if (n < 1881)   /* undefined below the offset */
        return (seq_t)0;

    size_t idx = (size_t)(n - 1881);
    if (idx >= digits_len)
        return (seq_t)0;

    return (seq_t)(digits[idx] - '0');
}
