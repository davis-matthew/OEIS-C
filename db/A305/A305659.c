/*
 * A305659  Powers of 2 in base 3/2.
 * https://oeis.org/A305659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A024629(A000079(n))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Powers of 2 in base 3/2 using the standard conversion algorithm. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Convert 2^n to its base‑3/2 representation. */
    seq_t N = pow2(n);

    /* The digits are generated least‑significant first. */
    size_t cap = 64;
    size_t len = 0;
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) return (seq_t)0;   /* allocation failure */

    while (N != (seq_t)0) {
        unsigned d = (unsigned)(N % (seq_t)3ull);   /* digit 0..2 */
        if (len == cap) {
            cap <<= 1;
            unsigned char *tmp = (unsigned char *)realloc(digits, cap);
            if (!tmp) {
                free(digits);
                return (seq_t)0;
            }
            digits = tmp;
        }
        digits[len++] = (unsigned char)d;

        N = (N - (seq_t)d) / (seq_t)3ull;
        N = N * (seq_t)2ull;
    }

    /* Assemble the decimal number from the base‑3/2 digits. */
    seq_t result = (seq_t)0;
    for (size_t i = len; i-- > 0; ) {
        result = result * (seq_t)10ull + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
