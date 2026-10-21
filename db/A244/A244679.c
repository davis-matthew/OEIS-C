/*
 * A244679  The spiral of Champernowne read by the Northeast ray.
 * https://oeis.org/A244679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit at position m in Champernowne’s constant, where m =
 *            4(n-1)^2 – 2(n-1) + 1 (n ≥ 1). The digit is taken from
 *            Champernowne’s infinite decimal 123456789101112…
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Precomputed powers of 10 up to 10^8 (enough for n ≤ 10000). */
static const idx_t pow10[9] = {
    1ULL,          /* 10^0 */
    10ULL,         /* 10^1 */
    100ULL,        /* 10^2 */
    1000ULL,       /* 10^3 */
    10000ULL,      /* 10^4 */
    100000ULL,     /* 10^5 */
    1000000ULL,    /* 10^6 */
    10000000ULL,   /* 10^7 */
    100000000ULL   /* 10^8 */
};

/* Return the idx‑th (1‑based) digit of Champernowne’s constant. */
static seq_t champernowne_digit(idx_t idx)
{
    idx_t remaining = idx;
    idx_t len = 1;          /* current digit length of numbers */
    idx_t count = 9;        /* how many numbers have current length */
    idx_t base = 1;         /* first number of current length */

    /* Skip complete blocks of digits. */
    while (remaining > len * count) {
        remaining -= len * count;
        len++;
        count *= 10;
        base *= 10;
    }

    /* Position within the current block. */
    idx_t number_offset = (remaining - 1) / len;   /* 0‑based */
    idx_t digit_index   = (remaining - 1) % len;   /* 0‑based from left */

    idx_t number = base + number_offset;           /* actual integer */

    /* Extract the digit. */
    idx_t pow = pow10[len - 1 - digit_index];
    return (seq_t)((number / pow) % 10);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n < 1; return 0 as a safe default */
        return (seq_t)0;

    idx_t k = n - 1;                     /* use k = n-1 for formula */
    idx_t m = 4 * k * k - 2 * k + 1;     /* position in Champernowne */
    return champernowne_digit(m);
}
