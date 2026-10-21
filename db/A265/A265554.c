/*
 * A265554  a(n) = smallest base-8 palindrome m >= n such that every base-8
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 10.
 * https://oeis.org/A265554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest base‑8 palindrome m ≥ n with digitwise * m[i] ≥
 *            n[i] (for each digit position i)
 * Range:     u8 n <= 67, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract base‑8 digits of n (least significant first). */
    unsigned digits[64];  /* enough for 64‑bit numbers (≤ 21 octal digits) */
    size_t len = 0;
    unsigned long long tmp = (unsigned long long)n;
    while (tmp > 0ULL) {
        digits[len++] = (unsigned)(tmp & 7ULL);   /* tmp % 8 */
        tmp >>= 3;                               /* tmp / 8 */
    }
    if (len == 0)          /* n == 0 */
        return (seq_t)0;

    /* Compute the minimal palindrome by taking digitwise maxima. */
    seq_t result = 0;
    for (long long i = (long long)len - 1; i >= 0; --i) {
        unsigned int left  = digits[(size_t)i];
        unsigned int right = digits[(size_t)(len - 1 - i)];
        unsigned int d = (left > right) ? left : right;
        result = result * (seq_t)8 + (seq_t)d;
    }
    return result;
}
