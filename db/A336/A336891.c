/*
 * A336891  Sequence following Van Eck's sequence unless a prime which was
 *          previously encountered is found, in which case, a(n+1) = n - m,
 *          where m is the largest possible value such that a(m) is also
 *          prime.
 * https://oeis.org/A336891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0. For n ≥ 1, let curr = a(n). If curr is a prime which
 *            has appeared before (< n), then a(n+1) = n - m where m is the
 *            largest index < n such that a(m) is prime. Otherwise, if curr
 *            has appeared before, a(n+1) = n - m where m is the largest index
 *            < n with a(m) = curr. If curr has not appeare...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq          = NULL; /* 1‑based array of sequence terms */
    static idx_t capacity      = 0;    /* allocated size of seq             */
    static idx_t len           = 0;    /* number of terms that have been
                                         * computed (max index present)     */
    static idx_t prev_prime    = 0;    /* largest prime index < current n   */

    if (n == 0) return 0;          /* safety: sequence is defined for n>=1 */

    /* allocate initial storage if needed */
    if (seq == NULL) {
        capacity = 8;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* initialise first term */
        seq[1] = (seq_t)0;
        len    = 1;
        prev_prime = 0;             /* a(1) is not prime */
    }

    /* extend capacity if target n is beyond current allocation */
    if (capacity <= n) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t current = seq[i - 1];   /* a(n) for current i-1 */

        /* find last index j < i-1 where seq[j] == current */
        idx_t last_same = 0;
        for (idx_t j = i - 2; j > 0; --j) {
            if (seq[j] == current) {
                last_same = j;
                break;
            }
        }

        bool current_is_prime = is_prime_big(current);
        seq_t next;

        if (current_is_prime && last_same > 0) {
            /* repeated prime: use the most recent prime index */
            next = (seq_t)(i - 1) - (seq_t)prev_prime;
        } else if (last_same > 0) {
            /* repeated non‑prime value */
            next = (seq_t)(i - 1) - (seq_t)last_same;
        } else {
            /* first occurrence of current value */
            next = (seq_t)0;
        }

        seq[i] = next;

        /* update prev_prime for future iterations */
        if (current_is_prime) {
            prev_prime = i - 1;
        }

        ++len;
    }

    return seq[n];
}
