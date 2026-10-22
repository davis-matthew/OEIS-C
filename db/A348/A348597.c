/*
 * A348597  a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k / (3*k)!.
 * https://oeis.org/A348597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (-1)^k / (3*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of factorials up to the maximum n seen so far. */
    static seq_t *fact = NULL;        /* factorial array */
    static idx_t fact_len = 0;        /* number of precomputed factorials (fact_len-1 is last index) */

    /* Ensure enough space for factorial up to n. */
    if ((idx_t)n + 1 > fact_len) {
        idx_t new_len = (idx_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, sizeof(seq_t) * new_len);
        if (!tmp) { /* In a real build add error handling; here just avoid crash. */
            exit(1);
        }
        fact = tmp;
        /* Compute new factorial entries. The already existing entries are unchanged. */
        for (idx_t i = fact_len; i <= n; ++i) {
            if (i == 0) {
                fact[i] = (seq_t)1;
            } else {
                fact[i] = fact[i - 1] * (seq_t)i;
            }
        }
        fact_len = new_len;
    }

    /* Compute the sum using successive ratio to avoid recomputing large factorials. */
    seq_t term = fact[n];          /* n! / (3*0)!   */
    seq_t res  = 0;
    int sign = 1;                  /* +1 for positive terms, -1 for negative terms */

    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        if (sign > 0)
            res += term;
        else
            res -= term;          /* safe: term <= current res because terms decrease */

        /* Prepare next term if there is one. */
        if (k == maxk)
            break;

        /* Denominator = (3k+1)*(3k+2)*(3k+3) */
        seq_t d = (seq_t)(3 * k + 1);
        d *= (seq_t)(3 * k + 2);
        d *= (seq_t)(3 * k + 3);

        term /= d;
        sign = -sign;             /* flip sign for next k */
    }

    return res;
}
