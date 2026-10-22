/*
 * A356476  Decimal expansion of Loschmidt constant in m^-3 (273.15 K, 100
 *          kPa).
 * https://oeis.org/A356476
 *
 * Offset:    26    (a(26) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th decimal digit of the rational number
 *            (2*10^35)/7542485487. The OEIS offset is 26, so a(26) is the
 *            first digit of the integer part, followed by the remaining
 *            digits of the integer part and then the recurring fractional
 *            digits
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

static seq_t int_digits[50];
static size_t int_len = 0;

/* Cache for fractional digits. */
static seq_t *frac_digits = NULL;
static size_t frac_len = 0, frac_cap = 0;

/* Constants for the division. */
static const unsigned long long DENOM = 7542485487ULL;
static unsigned __int128 numer = 0;      /* 2*10^35 */
static unsigned __int128 rem  = 0;        /* remainder after integer part */
static unsigned __int128 denom128 = 0;
static int initialized = 0;

/* Make room in the fractional digit buffer. */
static void ensure_frac_cap(size_t needed)
{
    if (needed <= frac_cap) return;
    size_t new_cap = frac_cap == 0 ? 128 : frac_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    frac_digits = (seq_t *)realloc(frac_digits, new_cap * sizeof(seq_t));
    frac_cap = new_cap;
}

/* Perform the heavy one‑time initialisation. */
static void init(void)
{
    if (initialized) return;
    initialized = 1;

    /* numer = 2 * 10^35. */
    unsigned __int128 pow10 = 1;
    for (int i = 0; i < 35; ++i) pow10 *= 10;
    numer = 2 * pow10;

    denom128 = (unsigned __int128)DENOM;

    /* Integer part and remainder. */
    unsigned __int128 int_part = numer / denom128;
    rem = numer % denom128;

    /* Convert integer part to decimal digits (most significant first). */
    unsigned __int128 tmp = int_part;
    size_t pos = 0;
    if (tmp == 0) {
        int_digits[pos++] = 0;
    } else {
        while (tmp > 0) {
            int_digits[pos++] = (seq_t)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to most significant first */
        for (size_t i = 0; i < pos / 2; ++i) {
            seq_t t = int_digits[i];
            int_digits[i] = int_digits[pos - 1 - i];
            int_digits[pos - 1 - i] = t;
        }
    }
    int_len = pos;
}

/* Return the digit at position n. */
seq_t a(idx_t n)
{
    if (n < 26) return (seq_t)0;          /* undefined below the offset */

    idx_t pos = n - 25;                    /* 1‑based position in digit stream */
    init();

    if ((size_t)pos <= int_len) {          /* integer part */
        return int_digits[pos - 1];
    }

    /* Fractional part. */
    idx_t frac_needed = pos - int_len;     /* 1‑based index in fractional digits */
    if ((size_t)frac_needed > frac_len) {
        ensure_frac_cap(frac_needed);
        while (frac_len < (size_t)frac_needed) {
            rem *= 10;
            seq_t digit = (seq_t)(rem / denom128);
            rem = rem % denom128;
            frac_digits[frac_len++] = digit;
        }
    }

    return frac_digits[frac_needed - 1];
}
