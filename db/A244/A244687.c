/*
 * A244687  The spiral of Champernowne read by the Southwest ray.
 * https://oeis.org/A244687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit at position 4*(n-1)^2 + 2*(n-1) + 1 in Champernowne
 *            constant
 * Range:     u8 n <= 13, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Powers of 10 up to 10^10, indexed by number of digits-1 (e.g. pow10[3] == 10^3 ) */
static const idx_t pow10[11] = {
    1ULL,          /* 10^0 */
    10ULL,         /* 10^1 */
    100ULL,        /* 10^2 */
    1000ULL,       /* 10^3 */
    10000ULL,      /* 10^4 */
    100000ULL,     /* 10^5 */
    1000000ULL,    /* 10^6 */
    10000000ULL,   /* 10^7 */
    100000000ULL,  /* 10^8 */
    1000000000ULL, /* 10^9 */
    10000000000ULL/* 10^10 (unused but harmless) */
};

/* Return the decimal digit (0‑9) at the given 1‑based position in the Champernowne constant. */
static seq_t cham_digit_at_pos(idx_t pos)
{
    /* Find the number of decimal digits d of the integer that contains pos. */
    idx_t d = 1;
    idx_t cumulative = 0;
    while (true) {
        idx_t count_in_this_len = 9 * pow10[d - 1];
        idx_t digits_contributed = count_in_this_len * d;
        if (pos <= cumulative + digits_contributed)
            break;
        cumulative += digits_contributed;
        d++;
    }

    /* Offset within the block of d‑digit numbers (0‑based). */
    idx_t offset = pos - cumulative - 1;
    /* The integer that contains the digit (as seq_t). */
    seq_t number = (seq_t)pow10[d - 1] + (seq_t)(offset / d);
    /* Index of the digit within that integer (0‑based from the left). */
    idx_t digit_index = offset % d;
    /* 10^(d-1-digit_index) as seq_t */
    seq_t pow_div = (seq_t)pow10[d - 1 - digit_index];
    /* Extract the digit */
    seq_t digit = (number / pow_div) % (seq_t)10;
    return digit;
}

seq_t a(idx_t n)
{
    /* Compute the 1‑based position along the southwest ray. */
    idx_t k = n - 1;
    idx_t pos = 4 * k * k + 2 * k + 1;
    return cham_digit_at_pos(pos);
}
