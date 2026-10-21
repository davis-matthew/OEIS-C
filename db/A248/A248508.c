/*
 * A248508  Decimal expansion of K_J = 2 e/h (Josephson constant) in units of
 *          A s^2 kg^{-1} m^{-2} in the 2019 SI system of units.
 * https://oeis.org/A248508
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits of 2*e/h (Josephson constant).  The sequence is
   defined as a(n) = the (n-15)-th decimal digit of the exact value
   2*e/h.  The first term listed (a(15)) is the integer part 4. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The rational value 2*e/h = 3204353268 / 662607015. */
    static const uint64_t NUM = 3204353268ULL;
    static const uint64_t DEN = 662607015ULL;

    /* Cache for the computed decimal digits.  digits[0] = integer part (4),
       digits[1] = first decimal digit (8), etc. */
    static seq_t *digits = NULL;
    static size_t cap = 0;    /* allocated capacity */
    static size_t len = 0;    /* number of digits stored in 'digits' */

    /* Non‑negative remainder during long division.  Needed only while
       generating digits beyond the integer part. */
    static uint64_t rem = 0;
    static int initialized = 0;

    if (n < 15) {
        /* Sequence is defined only for n >= 15 in the specification. */
        return 0;
    }

    idx_t idx = n - 15;          /* zero‑based index into 'digits' */

    /* Ensure the cache is filled up to 'idx' */
    if (!initialized) {
        /* Compute integer part once */
        uint64_t intpart = NUM / DEN;
        rem = NUM % DEN;

        /* Allocate minimal initial capacity */
        cap = 16;
        digits = (seq_t *)malloc(sizeof(seq_t) * cap);
        digits[0] = (seq_t)intpart;
        len = 1;
        initialized = 1;
    }

    /* Grow the array if necessary */
    if ((size_t)idx >= cap) {
        size_t new_cap = cap * 2;
        if ((size_t)idx >= new_cap)
            new_cap = (size_t)idx + 1;
        seq_t *new_ptr = (seq_t *)realloc(digits, sizeof(seq_t) * new_cap);
        if (!new_ptr) { /* allocation failed – return 0 (unlikely) */
            return 0;
        }
        digits = new_ptr;
        cap = new_cap;
    }

    /* Generate digits until we have one at position 'idx' */
    while (len <= idx) {
        rem *= 10;                       /* Bring down next digit */
        seq_t d = (seq_t)(rem / DEN);    /* Next decimal digit */
        rem %= DEN;                      /* Update remainder */
        digits[len++] = d;
    }

    return digits[idx];
}
