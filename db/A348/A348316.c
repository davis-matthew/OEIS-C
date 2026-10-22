/*
 * A348316  a(n) is the largest Niven (or Harshad) number with exactly n
 *          digits and not containing the digit 0.
 * https://oeis.org/A348316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the largest n‑digit Harshad (Niven) number that contains no
   digit 0. The algorithm starts with the number consisting of n nines
   (10^n − 1) and decrements it until a suitable candidate is found. */

/* external helpers */
/* in the arbitrary‑precision build seq_t is a class that supports
   + and *. In the fixed‑size builds seq_t is an unsigned integer type
   that also supports + and *. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 according to the offset; behaviour for n < 1 is irrelevant */
    size_t N = (size_t)n;                     /* number of digits */
    uint8_t *dig = (uint8_t *)malloc(N);      /* array of decimal digits */
    if (!dig) return 0;                       /* should never happen */

    /* initialise with all 9's: 99…9 */
    for (size_t i = 0; i < N; ++i) dig[i] = 9U;

    /* iterate downward until a Harshad number without zeros is found */
    for (;;) {
        /* compute digit sum and check for zeros */
        uint32_t sum = 0;
        bool hasZero = false;
        for (size_t i = 0; i < N; ++i) {
            uint8_t d = dig[i];
            if (d == 0) { hasZero = true; break; }
            sum += d;
        }

        /* if no zeros and sum > 0, test divisibility */
        if (!hasZero && sum > 0) {
            uint32_t rem = 0; /* value modulo sum */
            for (size_t i = 0; i < N; ++i) {
                rem = ( ((uint64_t)rem * 10) + dig[i] ) % sum;
            }
            if (rem == 0) {
                /* build the seq_t value from the digit array */
                seq_t val = 0;
                for (size_t i = 0; i < N; ++i) {
                    val = val * 10 + (seq_t)dig[i];
                }
                free(dig);
                return val;
            }
        }

        /* decrement the current number by 1 (borrow across digits) */
        size_t pos = N - 1;
        while (pos < N) {              /* loop until borrow handled */
            if (dig[pos] > 0) {
                dig[pos]--;
                break;
            } else {
                dig[pos] = 9U;
                if (pos == 0) break;  /* should never happen before solution */
                --pos;
            }
        }
    }

    /* unreachable, but keeps compiler happy */
    free(dig);
    return 0;
}
