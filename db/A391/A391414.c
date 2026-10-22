/*
 * A391414  a(n) is the median of the set of the distinct values of (n-1)^n,
 *          (n-1)^(n+1), n^(n-1), n^(n+1), (n+1)^(n-1), (n+1)^n.
 * https://oeis.org/A391414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = median of distinct values (n-1)^n, (n-1)^(n+1), n^(n-1),
 *            n^(n+1), (n+1)^(n-1), (n+1)^n For n ≥ 9 this equals ((n-1)^n +
 *            (n+1)^n)/2, but the median is computed generically for all n >=
 *            1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t v[6];
    seq_t n_minus  = (seq_t)(n - (idx_t)1);
    seq_t n_plus   = (seq_t)(n + (idx_t)1);
    seq_t n_seq    = (seq_t)n;

    v[0] = seq_pow(n_minus, n);                  /* (n-1)^n        */
    v[1] = seq_pow(n_minus, n + (idx_t)1);       /* (n-1)^(n+1)    */
    v[2] = seq_pow(n_seq, n - (idx_t)1);         /* n^(n-1)        */
    v[3] = seq_pow(n_seq, n + (idx_t)1);         /* n^(n+1)        */
    v[4] = seq_pow(n_plus, n - (idx_t)1);        /* (n+1)^(n-1)    */
    v[5] = seq_pow(n_plus, n);                   /* (n+1)^n        */

    /* insertion sort (6 elements only) */
    for (idx_t i = 1; i < 6; ++i) {
        seq_t key = v[i];
        idx_t j = i;
        while (j > 0 && v[j-1] > key) {
            v[j] = v[j-1];
            --j;
        }
        v[j] = key;
    }

    /* dedupe keeping order */
    seq_t uniq[6];
    size_t uniq_len = 0;
    for (idx_t i = 0; i < 6; ++i) {
        if (uniq_len == 0 || v[i] != uniq[uniq_len-1]) {
            uniq[uniq_len++] = v[i];
        }
    }

    /* median of distinct values */
    if (uniq_len & 1) {  /* odd number of values */
        return uniq[uniq_len/2];
    } else {             /* even number of values */
        return (uniq[uniq_len/2 - 1] + uniq[uniq_len/2]) / (seq_t)2;
    }
}
