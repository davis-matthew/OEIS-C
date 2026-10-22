/*
 * A324034  Decimal expansion of the conversion factor from the old derived SI
 *          unit for energy J (joule) to the combination of 2019 SI units
 *          h*Δν_{Cs}.
 * https://oeis.org/A324034
 *
 * Offset:    23    (a(23) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the (n-22)-th digit of the decimal expansion of 10^42 /
 *            (662607015*9192631770)
 * Range:     u8 n <= 123, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t D = 6091102297113866550ULL; /* 662607015 * 9192631770 */

#define MAX_DIGITS 12000 /* enough for n up to ~10000 */

static seq_t digits[MAX_DIGITS];
static size_t len = 0;
static unsigned __int128 rem = 0;
static int init_done = 0;

static void init_seq(void)
{
    /* compute Q = floor(10^42 / D) and the remainder R */
    unsigned __int128 r = 0;
    seq_t temp[50];
    size_t temp_len = 0;

    /* dividend "1" followed by 42 zeros -> 43 digits */
    for (size_t i = 0; i < 43; ++i) {
        uint8_t d = (i == 0) ? 1 : 0;
        r = r * 10 + d;
        seq_t q = (seq_t)(r / D);
        r = r % D;
        temp[temp_len++] = q;
    }

    /* trim leading zeros of the quotient */
    size_t first = 0;
    while (first < temp_len - 1 && temp[first] == 0)
        ++first;
    len = temp_len - first;

    for (size_t i = 0; i < len; ++i)
        digits[i] = temp[first + i];

    rem = r;            /* remainder after the integer part */
    init_done = 1;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_seq();

    /* sequence is defined only for n >= 23 */
    if (n < 23)
        return (seq_t)0;   /* arbitrary, unused */

    size_t pos = (size_t)(n - 23);   /* 0‑based position in the digit stream */

    if (pos >= MAX_DIGITS)
        return (seq_t)0;   /* out of bounds, should not happen for tested ranges */

    while (len <= pos) {
        /* generate next fractional digit from the current remainder */
        rem = rem * 10;
        seq_t d = (seq_t)(rem / D);
        rem = rem % D;
        digits[len++] = d;
    }

    return digits[pos];
}
