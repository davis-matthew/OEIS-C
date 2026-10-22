/*
 * A386523  Decimal expansion of 577/408.
 * https://oeis.org/A386523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-16) for n > 20; the first 20 terms are the decimal *
 *            expansion of 577/408
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t digits[20];
    static int init = 0;
    if (!init) {
        unsigned long long num = 577ULL;
        unsigned long long den = 408ULL;
        unsigned long long int_part = num / den;
        unsigned long long rem     = num % den;
        digits[0] = (seq_t)int_part;
        for (int i = 1; i < 20; ++i) {
            rem = rem * 10ULL;
            unsigned long long digit = rem / den;
            rem = rem % den;
            digits[i] = (seq_t)digit;
        }
        init = 1;
    }

    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    if (n <= 20)
        return digits[n - 1];

    /* For n > 20 the period of length 16 starts at a(5). */
    idx_t pos = 4 + ((n - 5) % 16); /* 0‑based index into digits array */
    return digits[pos];
}
