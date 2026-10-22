/*
 * A325977  a(n) = (1/2)*(A034448(n)+A048250(n)-2*n), where A034448 is the sum
 *          of unitary divisors and A048250 is the sum of squarefree divisors.
 * https://oeis.org/A325977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/2)*(A034448(n)+A048250(n)-2*n)
 * Range:     i8 n <= 83, i16 n <= 9822, i32 n <= 9822, i64 n <= 9822,
 *            i128 n <= 9822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset starts at 1, just in case */
    seq_t n_seq = (seq_t)n;

    seq_t unitary_prod = 1;   /* product of (1 + p^e) over prime factors */
    seq_t squarefree_prod = 1;/* product of (1 + p) over distinct primes */

    idx_t temp = n;
    idx_t d = 2;

    while (d <= temp / d) {                     /* trial division */
        if (temp % d == 0) {
            idx_t exp = 0;
            seq_t p_pow = 1;                     /* p^exp */
            while (temp % d == 0) {
                temp /= d;
                exp++;
                p_pow *= (seq_t)d;              /* multiply by p */
            }
            unitary_prod *= (p_pow + 1);        /* (1 + p^e) */
            squarefree_prod *= ((seq_t)d + 1);  /* (1 + p)            */
        }
        d = (d == 2) ? 3 : d + 2;               /* after 2, skip even numbers */
    }

    if (temp > 1) { /* remaining prime factor */
        unitary_prod *= ((seq_t)temp + 1);
        squarefree_prod *= ((seq_t)temp + 1);
    }

    seq_t result = (unitary_prod + squarefree_prod - (seq_t)2 * n_seq) / (seq_t)2;
    return result;
}
