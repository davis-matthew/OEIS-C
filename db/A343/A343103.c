/*
 * A343103  a(n) is the sum of the number of times each digit in n (taken with
 *          repetition) has appeared in the sequence.
 * https://oeis.org/A343103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{digit d of the decimal representation of n} (number
 *            of times d has appeared in terms a[0] … a[n-1])
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_frequencies.h"

static seq_t        *SEQ   = NULL;   /* stored values of a(n) */
static size_t        SEQ_CAP = 0;    /* current capacity of SEQ */
static size_t        SEQ_LEN = 0;    /* number of values computed */

static seq_t         FREQ[10] = {0};  /* cumulative digit frequencies in the sequence so far */

/* public function required by the specification */
seq_t a(idx_t n)
{
    /* compute terms up to n if necessary */
    while (SEQ_LEN <= n) {
        /* ensure capacity */
        if (SEQ_LEN == 0) {
            if (SEQ_CAP == 0) {
                SEQ_CAP = 128;
                SEQ = (seq_t *)malloc(SEQ_CAP * sizeof(seq_t));
            }
        }
        if (SEQ_LEN >= SEQ_CAP) {
            SEQ_CAP <<= 1;                     /* double the capacity */
            SEQ = (seq_t *)realloc(SEQ, SEQ_CAP * sizeof(seq_t));
        }

        /* compute a(SEQ_LEN) */
        seq_t  val = 0;
        idx_t  x   = SEQ_LEN;                  /* current index */

        if (x == 0) {
            val += FREQ[0];
        } else {
            while (x > 0) {
                int d = x % 10;                /* extract decimal digit */
                val += FREQ[d];
                x   /= 10;
            }
        }

        SEQ[SEQ_LEN] = val;                    /* store the term */

        /* update cumulative digit frequencies with digits of val */
        unsigned int local[10] = {0};
        digit_frequencies(val, local);
        for (int i = 0; i < 10; ++i) {
            FREQ[i] += local[i];
        }

        ++SEQ_LEN;                            /* ready for next index */
    }

    return SEQ[n];                            /* return requested value */
}
