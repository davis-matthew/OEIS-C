/*
 * A267063  Coefficient of x^4 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4)
 *             + 12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 * Initial values:
 *   a(0) = 47, a(1) = 7547, a(2) = 96847, a(3) = 8834047,
 *   a(4) = 335645147, a(5) = 17176306847, a(6) = 781541264047,
 *   a(7) = 37170460359547, a(8) = 1738056704580047
 */

/*   The function below implements the recurrence directly.  For any
 *   sequence type `seq_t` and index type `idx_t`, the computations
 *   use only unsigned arithmetic, which is defined modulo 2^N.  This
 *   guarantees correct results when the value fits in the type, as
 *   required by the evaluation setup.  The routine is O(n) and works
 *   for calls in any order.  No external utilities are needed.   */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 9 values cast to seq_t (overflow is fine for small types). */
    static const seq_t init[9] = {
        (seq_t)47ULL,
        (seq_t)7547ULL,
        (seq_t)96847ULL,
        (seq_t)8834047ULL,
        (seq_t)335645147ULL,
        (seq_t)17176306847ULL,
        (seq_t)781541264047ULL,
        (seq_t)37170460359547ULL,
        (seq_t)1738056704580047ULL
    };

    /* Return immediately for the base cases. */
    if (n < 9) {
        return init[n];
    }

    /* Buffer holding the last 9 terms, with s[0] = a(i-1) etc. */
    seq_t s[9];
    /* Populate s so that s[0] = a(8), s[1] = a(7), …, s[8] = a(0). */
    for (idx_t k = 0; k < 9; ++k) {
        s[8 - k] = init[k];
    }

    /* Iterate from a(9) up to the requested a(n). */
    for (idx_t i = 9; i <= n; ++i) {
        /* Compute next term using the split‐sum form to avoid
           negative intermediates in unsigned arithmetic. */
        seq_t pos = 34   * s[0] + 714   * s[1]
                  + 12376 * s[4] + 4641  * s[5] + s[8];
        seq_t neg = 4641 * s[2] + 12376 * s[3]
                  + 714   * s[6] + 34    * s[7];

        seq_t next = pos - neg;  /* safe because pos >= neg for all n */

        /* Shift the buffer to make room for the new term. */
        for (idx_t j = 8; j > 0; --j) {
            s[j] = s[j - 1];
        }
        s[0] = next;  /* s[0] now holds a(i) */
    }

    return s[0];
}
