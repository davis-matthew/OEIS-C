/*
 * A244810  The hexagonal spiral of Champernowne, read along the North (or
 *          360-degree) ray.
 * https://oeis.org/A244810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12n^2-25n+14)-th digit of Champernowne's constant
 *            (1.234567891011…)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t pow10[20] = {
    1ULL,          10ULL,          100ULL,         1000ULL,
    10000ULL,      100000ULL,      1000000ULL,     10000000ULL,
    100000000ULL,  1000000000ULL,  10000000000ULL, 100000000000ULL,
    1000000000000ULL,10000000000000ULL,100000000000000ULL,
    1000000000000000ULL,10000000000000000ULL,100000000000000000ULL,
    1000000000000000000ULL
};

seq_t a(idx_t n) {
    /* Compute k = (12n^2 - 25n + 14) */
    idx_t k = (idx_t)12 * n * n - (idx_t)25 * n + 14ULL;

    idx_t remaining = k;
    idx_t d = 1;
    idx_t block = 0;
    /* Find digit length d where the k-th digit resides */
    while (1) {
        idx_t count = 9ULL * pow10[d - 1];
        block = count * d;
        if (remaining <= block)
            break;
        remaining -= block;
        ++d;
    }

    /* Now within d-digit numbers */
    idx_t offset = remaining - 1;          /* 0‑based offset within block */
    idx_t number_index = offset / d;       /* 0‑based index of the d‑digit number */
    idx_t digit_index = offset % d;        /* 0‑based index of desired digit (most significant = 0) */

    idx_t number = pow10[d - 1] + number_index;
    idx_t divisor = pow10[d - 1 - digit_index];
    seq_t digit = (seq_t)((number / divisor) % 10ULL);
    return digit;
}
