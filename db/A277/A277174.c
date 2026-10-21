/*
 * A277174  a(n) = Product_{i=1..n} i*rad(i) where rad(n) = A007947(n).
 * https://oeis.org/A277174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} i * rad(i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where rad(i) is the product of the distinct prime factors of i */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t limit = (size_t)n;

    // Allocate arrays for radicals and a simple is-composite marker
    unsigned int *rad = (unsigned int*)malloc((limit + 1) * sizeof(unsigned int));
    unsigned char *isComp = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!rad || !isComp) {
        free(rad);
        free(isComp);
        return (seq_t)0;          /* memory allocation failure */
    }

    rad[0] = 0;
    rad[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        rad[i] = 1;
        isComp[i] = 0;
    }

    // Sieve to compute the radical of every integer up to `limit`
    for (size_t p = 2; p <= limit; ++p) {
        if (!isComp[p]) {
            for (size_t j = p; j <= limit; j += p) {
                rad[j] *= (unsigned int)p;
                if (j > p)
                    isComp[j] = 1;
            }
        }
    }

    // Compute the product ∏_{i=1..n} i * rad(i)
    seq_t result = (seq_t)1;
    for (size_t i = 1; i <= limit; ++i) {
        seq_t term = (seq_t)i * (seq_t)rad[i];
        result *= term;
    }

    free(rad);
    free(isComp);
    return result;
}
