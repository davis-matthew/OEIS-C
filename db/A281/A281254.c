/*
 * A281254  Concatenate the decimal numbers n n-2 n-4 ...5 3 1 2 4 ... n-5 n-3
 *          n-1 if n is odd, or n n-2 n-4 ... 6 4 2 1 3 5 ... n-5 n-3 n-1 if n
 *          is even.
 * https://oeis.org/A281254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the integers 1…n in the specific * order
 *            described in A281254: * • if n is even: n, n-2, n-4, … , 2, 1,
 *            3, 5, … , n-1 * • if n is odd : n, n-2, n-4, … , 1, 2, 4, 6, … ,
 *            n-1 * The concatenation is performed in decimal
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* Helper: number of decimal digits of a 64‑bit unsigned integer. */
static int digit_len_ull(uint64_t x) {
    int d = 0;
    do {
        d++;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    if (n % 2 == 1) {                          /* n is odd */
        for (long long i = (long long)n; i >= 1; i -= 2) {
            int d = digit_len_ull((uint64_t)i);
            result = result * pow10((idx_t)d) + (seq_t)i;
        }
        for (long long i = 2; i <= (long long)n - 1; i += 2) {
            int d = digit_len_ull((uint64_t)i);
            result = result * pow10((idx_t)d) + (seq_t)i;
        }
    } else {                                   /* n is even */
        for (long long i = (long long)n; i >= 2; i -= 2) {
            int d = digit_len_ull((uint64_t)i);
            result = result * pow10((idx_t)d) + (seq_t)i;
        }
        for (long long i = 1; i <= (long long)n - 1; i += 2) {
            int d = digit_len_ull((uint64_t)i);
            result = result * pow10((idx_t)d) + (seq_t)i;
        }
    }

    return result;
}
