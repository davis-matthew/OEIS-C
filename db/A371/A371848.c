/*
 * A371848  Decimal expansion of 2^110503 - 1, the 29th Mersenne prime.
 * https://oeis.org/A371848
 *
 * Offset:    33265    (a(33265) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) is the decimal digit at position n of 2^110503 − 1 (most
 *            significant digit has index 33265)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Constants */
#define MERSENNE_EXP 110503
#define OFFSET        33265u
#define DIGIT_COUNT   33265u
#define WORD_BITS     32
#define BASE          1000000000u   /* 10^9 */

/* The digits of 2^110503 − 1, most‑significant digit first.
   digits[0] corresponds to a(33265). */
static uint8_t digits[DIGIT_COUNT];

/* Compute the digits of 2^MERSENNE_EXP − 1 and store in digits[]. */
static void init_digits(void)
{
    enum { WORDS = 3500 };          /* enough for 110503 bits */
    uint32_t big[WORDS];
    size_t len = 0;

    /* Start with 2^MERSENNE_EXP. */
    for (size_t i = 0; i < WORDS; ++i) big[i] = 0;
    size_t high_word  = MERSENNE_EXP / WORD_BITS;
    size_t high_shift = MERSENNE_EXP % WORD_BITS;
    big[high_word] = (uint32_t)1u << high_shift;
    len = high_word + 1;

    /* Subtract 1. */
    size_t i = 0;
    while (true) {
        if (big[i] == 0u) {
            big[i] = 0xFFFFFFFFu;
            ++i;
        } else {
            big[i] -= 1u;
            break;
        }
    }

    /* Temporary buffer for reversed decimal digits. */
    enum { MAX_REVDIGITS = 35000 };   /* > DIGIT_COUNT */
    uint8_t rev[MAX_REVDIGITS];
    size_t rev_len = 0;

    /* Repeatedly divide by BASE, storing remainders. */
    while (len > 0) {
        uint32_t rem = 0u;
        for (size_t j = len; j-- > 0; ) {
            uint64_t cur = ((uint64_t)rem << WORD_BITS) | (uint64_t)big[j];
            big[j] = (uint32_t)(cur / BASE);
            rem    = (uint32_t)(cur % BASE);
        }
        while (len > 0 && big[len - 1] == 0u) --len;

        /* Extract up to 9 decimal digits from rem. */
        for (int k = 0; k < 9; ++k) {
            rev[rev_len++] = (uint8_t)(rem % 10u);
            rem /= 10u;
        }
    }

    /* Determine true number of digits. */
    size_t ms_index = rev_len - 1;
    while (ms_index > 0 && rev[ms_index] == 0u) --ms_index;
    size_t total_digits = ms_index + 1;   /* should equal DIGIT_COUNT */

    /* Store digits in forward order (most significant first). */
    for (size_t d = 0; d < total_digits; ++d) {
        digits[d] = rev[ms_index - d];
    }
    if (total_digits < DIGIT_COUNT) {    /* padding if needed (should not occur) */
        for (size_t d = total_digits; d < DIGIT_COUNT; ++d) digits[d] = 0u;
    }
}

/* Public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n < OFFSET) {
        return 0;   /* undefined behavior below the offset */
    }

    static bool initialized = false;
    if (!initialized) {
        init_digits();
        initialized = true;
    }

    return (seq_t)digits[n - OFFSET];
}
