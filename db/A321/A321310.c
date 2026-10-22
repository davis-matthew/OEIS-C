/*
 * A321310  List of pairs of numbers with mirror symmetry (calculator-style
 *          numerals).
 * https://oeis.org/A321310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(2k) = k-th smallest positive integer whose decimal digits are
 *            confined to {0,1,2,5,8} and do not end with 0 (0 itself is the
 *            first element). a(2k+1) = mirror(a(2k)), where mirror reverses
 *            the decimal representation and replaces each digit by its
 *            calculator‑style mirror: 0↔0, 1↔1, 2↔5, 5↔2, 8↔8
 * Range:     u8 n <= 44, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is: 0,0,1,1,2,5,5,2,8,8,11,11,12,51,15,21,18,81,21,15,22,55,
   25,25,... */

#include <stdint.h>

/* Map each digit to its calculator mirror. */
static const uint8_t MIRROR_MAP[10] = {
    0, /* 0 -> 0 */
    1, /* 1 -> 1 */
    5, /* 2 -> 5 */
    0, /* 3 not used */
    0, /* 4 not used */
    2, /* 5 -> 2 */
    0, /* 6 not used */
    0, /* 7 not used */
    8, /* 8 -> 8 */
    0  /* 9 not used */
};

/* Allowed digits for first/last position (no leading or trailing 0). */
static const uint8_t FIRST_DIGITS[4] = {1, 2, 5, 8};
/* Allowed digits for middle positions (0 is allowed). */
static const uint8_t MIDDLE_DIGITS[5] = {0, 1, 2, 5, 8};

/* Compute the mirror of a number (calculator mirror). */
static inline seq_t make_mirror(seq_t x)
{
    seq_t r = 0;
    if (x == 0) return 0;
    while (x != 0) {
        uint8_t d = (uint8_t)(x % 10);
        r = r * 10 + MIRROR_MAP[d];
        x /= 10;
    }
    return r;
}

/* Produce the p‑th number in the sorted list of mirrorable numbers
   (p == 0 returns 0). */
static seq_t number_at_p(idx_t p)
{
    if (p == 0) return 0;

    /* p‑th element including 0 at position 0.  For p>0 we work with
       numbers >0, counting from 1. */
    idx_t q = p - 1;           /* zero‑based index among numbers >0 */

    /* Length 1: digits 1,2,5,8 (4 numbers). */
    idx_t remaining = q;
    if (remaining < 4) {
        return (seq_t)FIRST_DIGITS[remaining];
    }
    remaining -= 4;

    /* For lengths >=2: count(L) = 4*5^(L-2)*4 = 16*5^(L-2) */
    idx_t L = 2;
    idx_t pow5 = 1;            /* 5^(L-2) */
    while (1) {
        idx_t countL = 16 * pow5;
        if (remaining < countL) break;
        remaining -= countL;
        L++;
        pow5 *= 5;
    }

    /* remaining now indexes within length L. */
    seq_t num = 0;
    if (L == 1) {                /* not reached, handled above */
        /* nothing */
    } else {
        /* Determine positions of digits. */
        idx_t groupsize_first = pow5 * 4;   /* 5^(L-2)*4 */
        idx_t first_idx = remaining / groupsize_first;
        idx_t rem_after_first = remaining % groupsize_first;

        idx_t last_idx = rem_after_first % 4;
        idx_t middle_offset = rem_after_first / 4;   /* 0 .. pow5-1 */

        /* Build number from most significant to least. */
        num = (seq_t)FIRST_DIGITS[first_idx];

        /* Middle digits (L-2 positions) in base‑5. */
        if (L > 2) {
            uint8_t middle_digits[L - 2];
            for (int i = L - 3; i >= 0; --i) {
                middle_digits[i] = MIDDLE_DIGITS[middle_offset % 5];
                middle_offset /= 5;
            }
            for (int i = 0; i < L - 2; ++i) {
                num = num * 10 + middle_digits[i];
            }
        }

        num = num * 10 + (seq_t)FIRST_DIGITS[last_idx];
    }

    return num;
}

seq_t a(idx_t n)
{
    idx_t p = n / 2;          /* pair index */
    if ((n & 1) == 0) {       /* even n: return the number itself */
        return number_at_p(p);
    } else {                  /* odd n: return its mirror */
        return make_mirror(number_at_p(p));
    }
}
