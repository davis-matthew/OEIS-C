/*
 * A382068  Array read by ascending antidiagonals: A(n,m) is obtained by
 *          concatenating the digits of floor(n/m) with those of its
 *          fractional part up to the digits of the first period, where the
 *          leading and trailing 0's are omitted.
 * https://oeis.org/A382068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, tabl
 * Formula:   a(n) = A(i,j) where (i,j) are given by ascending-antidiagonal
 *            mapping; A(i,j) is formed by concatenating the decimal digits of
 *            floor(i/j) and the fractional part of i/j up to the first
 *            period, omitting leading zeros
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 152, u64 n <= 275,
 *            u128 n <= 1127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute A(i,j) as described above. */
static seq_t compute_A(idx_t i, idx_t j)
{
    /* Integer part */
    seq_t floor_part = (seq_t)(i / j);

    /* Remainder for fractional part */
    idx_t rem = i % j;
    if (rem == 0)
        return floor_part;            /* No fractional digits */

    /* Track remainders to detect the first period */
    int *seen = (int *)malloc((size_t)j * sizeof(int));
    if (!seen)
        return floor_part;
    for (idx_t k = 0; k < j; ++k)
        seen[k] = -1;

    /* Store digits of the fractional part */
    uint8_t *digits = (uint8_t *)malloc((size_t)j * sizeof(uint8_t));
    if (!digits) {
        free(seen);
        return floor_part;
    }

    idx_t pos   = 0;
    int    rem_i = (int)rem;
    while (rem != 0 && seen[rem_i] == -1) {
        seen[rem_i] = (int)pos;          /* Mark this remainder */
        rem *= 10;                       /* Shift left */
        uint8_t digit = (uint8_t)(rem / j); /* Next decimal digit */
        rem %= j;                        /* New remainder */
        rem_i = (int)rem;
        digits[pos++] = digit;            /* Store the digit */
    }

    /* Skip leading zeros only when the integer part is 0 */
    idx_t start = 0;
    if (floor_part == 0) {
        while (start < pos && digits[start] == 0)
            start++;
    }

    /* Concatenate digits to form the final result */
    seq_t result = floor_part;
    for (idx_t k = start; k < pos; ++k)
        result = result * 10 + (seq_t)digits[k];

    free(seen);
    free(digits);
    return result;
}

/* Map n to (i,j) and return A(i,j). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* Undefined for n < 1, safeguard */

    /* Find the diagonal s such that (s-1)*s/2 >= n */
    idx_t s = 2;
    while ( ((s - 1) * s) / 2 < n )
        ++s;

    idx_t prev = ((s - 2) * (s - 1)) / 2;   /* elements before diagonal s */
    idx_t p    = n - prev - 1;              /* position within diagonal (0‑based) */
    idx_t i    = s - 1 - p;                 /* row index (1‑based) */
    idx_t j    = s - i;                     /* column index (1‑based) */

    return compute_A(i, j);
}
