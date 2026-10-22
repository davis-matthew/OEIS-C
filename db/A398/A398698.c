/*
 * A398698  a(n) = numerator(Sum_{k=1..n} sigma(floor(n/k))/floor(n/k)), where
 *          sigma(n) = A000203(n) is the sum of divisors of n.
 * https://oeis.org/A398698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, changed
 * Formula:   a(n) = numerator( Σ_{k=1..n} sigma(⌊n/k⌋)/⌊n/k⌋ )
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 58, u64 n <= 214,
 *            u128 n <= 1005, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the reduced rational sum
   and returns it as a seq_t value. */
#include <stdint.h>
#include "../utils/sigma_of.h"

/* Euclidean algorithm for an unsigned seq_t type.  All
   arithmetic uses only operators permitted by the
   sequence‑type interface. */
static inline seq_t _gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1.  For safety, return 0
       if called with 0 (though the problem guarantees n >= 1). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t sum_num = (seq_t)0;   /* numerator of the accumulated sum   */
    seq_t sum_den = (seq_t)1;   /* denominator of the accumulated sum */

    /* Iterate over distinct values of floor(n/k).  For each
       value m = n / k, the count of k giving this value is
       (next - cur). */
    for (idx_t cur = (idx_t)1; cur <= n; ) {
        idx_t m = n / cur;                     /* floor(n/k) */
        idx_t nxt = n / m + (idx_t)1;           /* next k where value changes */
        idx_t count = nxt - cur;                /* how many k give this m */

        seq_t sigma = sigma_of(m);              /* σ(m) */
        seq_t cnt_seq = (seq_t)count;           /* count as seq_t */
        seq_t term_num = sigma * cnt_seq;       /* numerator part of term */
        seq_t term_den = (seq_t)m;              /* denominator part of term */

        /* Add term to current sum: (sum_num/sum_den) + (term_num/term_den) */
        seq_t new_num = sum_num * term_den + sum_den * term_num;
        seq_t new_den = sum_den * term_den;

        /* Reduce the fraction by dividing by the GCD. */
        seq_t g = _gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;

        cur = nxt;  /* move to next distinct value */
    }

    return sum_num;   /* Numerator of the final reduced sum */
}
