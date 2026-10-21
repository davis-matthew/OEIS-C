/*
 * A244685  The spiral of Champernowne read by the West ray.
 * https://oeis.org/A244685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit of Champernowne constant at index (2*(n-1)-1)^2 +
 *            5*(n-1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This conforms to OEIS A244685 – the West‑ray of the Champernowne spiral. */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computed powers of 10 up to 10^18 (more than enough for n <= 10000). */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Cumulative number of digits in Champernowne up to the last number of each digit‑length
   (d = 1 gives 9, d = 2 gives 189, etc.).  The array is large enough for 18‑digit numbers. */
static const uint64_t cum_digits[20] = {
    0ULL,                 /* d = 0 (unused, for easier indexing) */
    9ULL,                 /* up to 9 */
    189ULL,               /* up to 99 */
    2889ULL,              /* up to 999 */
    38889ULL,             /* up to 9999 */
    488889ULL,            /* up to 99999 */
    5888889ULL,           /* up to 999999 */
    68888889ULL,          /* up to 9999999 */
    788888889ULL,         /* up to 99999999 */
    8888888889ULL,        /* up to 999999999 */
    98888888889ULL,       /* up to 9999999999 */
    1088888888889ULL,     /* up to 99999999999 */
    11888888888889ULL,    /* up to 999999999999 */
    128888888888889ULL,   /* up to 9999999999999 */
    1388888888888889ULL,  /* up to 99999999999999 */
    14888888888888889ULL, /* up to 999999999999999 */
    158888888888888889ULL,/* up to 9999999999999999 */
    1688888888888888889ULL/* up to 99999999999999999 */
};

/* Return the decimal digit at position pos (1‑based) in the Champernowne constant. */
static seq_t champernowne_digit(uint64_t pos) {
    /* Find the digit length d such that cum_digits[d-1] < pos <= cum_digits[d] */
    int d = 1;
    while (cum_digits[d] < pos) {
        d++;
    }

    /* Offset within the block of d‑digit numbers */
    uint64_t offset = pos - cum_digits[d - 1] - 1;      /* 0‑based */
    uint64_t number = pow10[d - 1] + offset / d;        /* the integer containing the digit */
    int digit_offset = offset % d;                      /* which digit in that number */

    /* Extract the digit from left to right */
    uint64_t divisor = pow10[d - 1 - digit_offset];
    uint64_t digit = (number / divisor) % 10ULL;

    return (seq_t)digit;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* n is at least 1 as per offset */
    uint64_t t = (uint64_t)(n - 1ULL);         /* t = n-1 */
    uint64_t i = (2ULL * t - 1ULL) * (2ULL * t - 1ULL) + 5ULL * t;  /* index in Champernowne */
    return champernowne_digit(i);
}
