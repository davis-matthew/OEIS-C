/*
 * A244112  Reverse digit count of n in decimal representation.
 * https://oeis.org/A244112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) concatenates the digit-count pairs (count of digit d)(digit
 *            d) for every decimal digit d that occurs in n, processed in
 *            descending order of d (from 9 down to 0)
 * Range:     u8 n <= 9, u16 n <= 101, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned char cnt[10] = {0};

    /* Count the decimal digits of n */
    if (n == 0) {
        cnt[0] = 1;                     /* special case for zero */
    } else {
        idx_t x = n;
        while (x > 0) {
            unsigned int d = x % 10;     /* current digit */
            cnt[d] += 1;
            x /= 10;
        }
    }

    /* Build the result by appending count digits and the digit itself */
    seq_t result = 0;
    for (int d = 9; d >= 0; --d) {
        unsigned int c = cnt[d];
        if (c == 0) continue;

        /* Convert the count 'c' to its decimal digits */
        unsigned char digits[32];
        int len = 0;
        if (c == 0) {
            digits[len++] = 0;
        } else {
            unsigned int tmp = c;
            while (tmp > 0) {
                digits[len++] = tmp % 10;
                tmp /= 10;
            }
        }

        /* Append the digits of the count (most–significant first) */
        for (int i = len - 1; i >= 0; --i) {
            result = result * 10 + digits[i];
        }
        /* Append the digit itself */
        result = result * 10 + d;
    }
    return result;
}
