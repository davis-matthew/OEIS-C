/*
 * A304174  Decimal expansion of 2^61/(3^32*993), the conjectured maximal
 *          residue in the Collatz 3x+1 problem.
 * https://oeis.org/A304174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of 2^61/(3^32*993) (offset 1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* One‑time computation of constants and digit cache */
    static seq_t N, D;
    static seq_t *digits = NULL;
    static size_t capacity = 0, len = 0;
    static seq_t last_rem = 0;
    static bool inited = false;

    if (!inited) {
        /* Compute N = 2^61     (allocating safely for any seq_t size) */
        N = 1;
        for (idx_t i = 0; i < 61; ++i)
            N = N * 2;

        /* Compute D = 3^32 * 993 */
        D = 993;
        for (idx_t i = 0; i < 32; ++i)
            D = D * 3;

        /* Allocate cache for digits, start with capacity 16 */
        capacity = 16;
        digits = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!digits)
            exit(1);                    /* allocation failure */

        /* Integer part and first remainder */
        digits[0] = N / D;              /* a(1) */
        last_rem   = N % D;
        len = 1;
        inited = true;
    }

    /* If the requested digit is already cached, return it */
    if ((size_t)n <= len)
        return digits[n - 1];

    /* Grow the cache and compute further digits as needed */
    while (len < (size_t)n) {
        if (len == capacity) {          /* double the storage when full */
            capacity *= 2;
            seq_t *tmp = (seq_t *)realloc(digits, sizeof(seq_t) * capacity);
            if (!tmp) {
                free(digits);
                exit(1);                /* reallocation failure */
            }
            digits = tmp;
        }
        last_rem = last_rem * 10;
        seq_t digit = last_rem / D;
        last_rem   %= D;
        digits[len] = digit;            /* store next decimal digit */
        ++len;
    }

    return digits[n - 1];
}
