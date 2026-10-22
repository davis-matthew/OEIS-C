/*
 * A365276  Sum of all prime substrings of n in base 10, including n itself
 *          and duplicate or overlapping substrings but not substrings with a
 *          leading 0.
 * https://oeis.org/A365276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A365276: sum of all prime substrings of n in base 10,
   including n itself and duplicate/overlapping substrings, but
   excluding substrings that begin with a leading 0. */

#include <stdbool.h>

/* Small primality test for values that arise as decimal substrings
   of n.  The outer loop will use only integers that fit in seq_t
   for the allowed n (tests up to at least 10000). */
static bool is_prime_small(seq_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (seq_t i = 3; i <= x / i; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Extract the decimal digits of n into an array in most-significant
       digit first order.  The array is big enough for the largest n that
       may be used in the tests. */
    unsigned char digits[32];
    int len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        /* Gather digits in reverse order first. */
        while (tmp > 0) {
            digits[len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
        /* Reverse to obtain the correct order. */
        for (int i = 0, j = len - 1; i < j; ++i, --j) {
            unsigned char t = digits[i];
            digits[i] = digits[j];
            digits[j] = t;
        }
    }

    seq_t sum = 0;

    /* Enumerate all substrings starting at each position i. */
    for (int i = 0; i < len; ++i) {
        /* Skip substrings with a leading zero. */
        if (digits[i] == 0)
            continue;

        seq_t val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + digits[j];
            if (is_prime_small(val))
                sum += val;
        }
    }

    return sum;
}
