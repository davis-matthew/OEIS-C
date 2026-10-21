/*
 * A243972  Least number k such that 2^k contains exactly n identical digits.
 * https://oeis.org/A243972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: repeatedly double the decimal representation of 2^k and
   record the first exponent at which each digit frequency occurs.  */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* cache[max_n] holds the minimal exponent for which the
       digit frequency max_n was first seen.
       The array is sized sufficiently for the expected range */
    static seq_t cache[200001];
    static uint8_t digits[20000];
    static size_t len = 1;          /* current number of decimal digits */
    static idx_t cur_k = 0;         /* exponent of current number (2^cur_k) */
    static int initialized = 0;

    if (!initialized) {
        /* start with 2^0 = 1 */
        memset(cache, 0, sizeof(cache));
        digits[0] = 1;
        len = 1;
        cur_k = 0;
        initialized = 1;
    }

    /* guard against requesting an index beyond the cache */
    if ((size_t)n >= sizeof(cache)/sizeof(cache[0]))
        return 0;   /* undefined for this implementation */

    if (cache[n] == 0) {
        unsigned freq[10];
        while (cache[n] == 0) {
            /* advance to next exponent */
            cur_k++;

            /* multiply current number by 2 */
            int carry = 0;
            for (size_t i = 0; i < len; ++i) {
                int v = digits[i] * 2 + carry;
                digits[i] = v % 10;
                carry = v / 10;
            }
            while (carry) {
                if (len >= sizeof(digits)) return 0;  /* safety guard */
                digits[len++] = carry % 10;
                carry /= 10;
            }

            /* count digit frequencies */
            memset(freq, 0, sizeof(freq));
            for (size_t i = 0; i < len; ++i)
                ++freq[digits[i]];

            /* record first occurrence of each frequency */
            for (int d = 0; d < 10; ++d) {
                int f = freq[d];
                if (f > 0 && (size_t)f < sizeof(cache)/sizeof(cache[0]) &&
                    cache[f] == 0)
                    cache[f] = cur_k;
            }
        }
    }

    return cache[n];
}
