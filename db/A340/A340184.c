/*
 * A340184  n with the rightmost occurrence of the smallest digit of n
 *          deleted.
 * https://oeis.org/A340184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, dumb, nonn
 * Formula:   a(n) = n with the rightmost occurrence of the smallest digit of
 *            n deleted
 * Range:     u8 n <= 1255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Little‑endian array of decimal digits (least significant first) */
    uint8_t digits[64];
    int len = 0;
    idx_t t = n;

    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t > 0) {
            digits[len++] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    /* Find the minimum digit */
    uint8_t min_d = 9;
    for (int i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        if (d < min_d) {
            min_d = d;
        }
    }

    /* Find the rightmost (least‑significant) occurrence of that minimum digit */
    int rightmostIdx = -1;
    for (int i = 0; i < len; ++i) {
        if (digits[i] == min_d) {
            rightmostIdx = i;
            break;
        }
    }

    /* Reconstruct the number without that digit */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        if (i == rightmostIdx) {
            continue;
        }
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
