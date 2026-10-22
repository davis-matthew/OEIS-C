/*
 * A375461  Self-consecutive numbers: numbers m such that there exists a
 *          nonnegative integer k for which the concatenation of m, m+1, ...,
 *          m+k is an m-digit number.
 * https://oeis.org/A375461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 8034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‑based implementation of OEIS A375461 (Self‑consecutive numbers) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return the n‑th term of A375461.  The sequence is generated lazily; on
   each call, it scans ascending numbers until it has found at least n
   terms.  No assumptions are made about the order of calls. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;  /* memoised terms */
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* storage capacity */
    static seq_t next_m     = 1;   /* next candidate to test */

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Count decimal digits of next_m (d >= 1) */
        int d = 0;
        seq_t tmp = next_m;
        do {
            tmp /= (seq_t)10;
            ++d;
        } while (tmp > 0);

        /* Compute 10^d */
        seq_t pow10 = 1;
        for (int i = 0; i < d; ++i)
            pow10 *= (seq_t)10;

        seq_t d_seq   = (seq_t)d;
        seq_t d_plus1 = d_seq + 1;

        /* Condition 1: m divisible by d AND m < 10^d * d/(d+1)  */
        bool cond1 = ((next_m % d_seq) == 0) &&
                     ((next_m * d_plus1) < (pow10 * d_seq));

        /* Condition 2: 10^d divisible by d+1 AND m >= 10^d * d/(d+1)  */
        bool cond2 = ((pow10 % d_plus1) == 0) &&
                     (next_m >= (pow10 * d_seq) / d_plus1);

        if (cond1 || cond2) {
            /* Grow storage if necessary */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(1);              /* out of memory */
                terms = tmp;
            }
            terms[terms_len++] = next_m;
        }
        ++next_m;
    }

    return terms[n - 1];
}
