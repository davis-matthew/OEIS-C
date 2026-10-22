/*
 * A392510  Decimal expansion of the total number of possible books in the
 *          fictional library described in the short story "The Library of
 *          Babel" by J. L. Borges (1941).
 * https://oeis.org/A392510
 *
 * Offset:    1834098    (a(1834098) is the first term)
 * Keywords:  nonn, cons, fini, easy
 * Formula:   a(n) = decimal digit of 25^1312000 at position n-1834098
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   big_pow, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(1834098) is the most significant digit of 25^1312000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/big_pow.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* The offset used by OEIS for this sequence */
    const idx_t OFFSET = 1834098ULL;

    /* The decimal representation of 25^1312000 (i.e. 5^2624000) */
    static char *digits = NULL;          /* string of digits */
    static size_t digits_len = 0;         /* length of the string */
    static bool initialized = false;     /* one‑time init flag */

    if (!initialized) {
        /* Compute 5^2624000 once */
        const seq_t BASE   = (seq_t)5;
        const seq_t EXP    = (seq_t)2624000ULL;   /* 2 * 1312000 */
        seq_t big = big_pow(BASE, EXP);

        /* First determine the required buffer size */
        size_t needed = 0;
        seq_to_str(big, NULL, &needed);
        digits_len = needed;

        /* Allocate buffer for the decimal string */
        digits = (char *)malloc(needed + 1);   /* +1 for terminating NUL */
        if (digits == NULL) {
            /* Allocation failed – return zero for all subsequent calls */
            return (seq_t)0;
        }

        /* Convert the big integer to decimal string */
        seq_to_str(big, digits, &needed);
        digits[needed] = '\0';   /* NUL terminator for safety */

        /* Mark as initialized */
        initialized = true;
    }

    /* Out‑of‑bounds indices (below the offset) are not required to be
       handled correctly; we return 0 in that case. */
    if (n < OFFSET)
        return (seq_t)0;

    /* 0‑based offset of the requested digit in the string */
    idx_t pos = n - OFFSET;
    if ((size_t)pos >= digits_len)
        return (seq_t)0;   /* position beyond the number – should not happen */

    /* Convert character digit to numeric value */
    return (seq_t)(digits[(size_t)pos] - '0');
}
