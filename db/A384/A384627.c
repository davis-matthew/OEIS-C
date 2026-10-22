/*
 * A384627  Decimal expansion of 1/998001.
 * https://oeis.org/A384627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th decimal digit of 1/998001.  
 * Since 1/998001 = 1/(999^2) has a pure recurring period of length 2997,
 * a(n) = period[n mod 2997] where period is the sequence of digits
 * obtained by long division of 1 by 998001.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute and cache the 2997‑digit repeating period once. */
static seq_t *period = NULL;      /* array of digits                */
static size_t period_len = 0;     /* number of digits (should be 2997) */

seq_t a(idx_t n)
{
    /* Lazy initialisation of the period array. */
    if (period == NULL) {
        /* The period length is known to be 2997. Allocate a bit more to
           be safe with compilers that may consider small overflow.   */
        const size_t buffer = 3000;
        period = (seq_t*)malloc(buffer * sizeof(seq_t));
        if (period == NULL) {
            /* In a real library we would abort or handle the error,
               but for the purposes of the exercise we skip error
               handling. */
            return 0;
        }

        /* Generating the period by long division. */
        const unsigned long long denom = 998001ULL;
        unsigned long long rem = 1ULL;               /* initial remainder */
        size_t idx = 0;

        do {
            rem *= 10ULL;
            seq_t digit = (seq_t)(rem / denom);      /* 0-9 fits in seq_t */
            period[idx++] = digit;
            rem %= denom;
        } while (rem != 1ULL);

        period_len = idx;   /* should be 2997, but we store it generically */
    }

    /* Return the requested digit (index starts at 0). */
    size_t pos = (size_t)(n % period_len);
    return period[pos];
}
