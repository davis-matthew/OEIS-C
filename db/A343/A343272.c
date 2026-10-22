/*
 * A343272  Partial sums of A170842.
 * https://oeis.org/A343272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of sequence A170842.
   a(n) = sum_{m=0..n} b(m), where the generating function of
   b(m) is Prod_{k>=1} (1 + 2*x^(2^k-1) + 3*x^(2^k)).
   Therefore a(n) is the n-th coefficient of 1/(1-x) times that product.
*/

#include <stdlib.h>
#include <stdint.h>

/* Computes a(n) for the sequence A343272. */
seq_t a(idx_t n)
{
    /* Allocate provisional working space */
    static seq_t *coeff  = NULL;
    static seq_t *tmp    = NULL;
    static size_t cap    = 0;
    idx_t max_n = n;
    /* Resize if necessary */
    if ((size_t)max_n + 1 > cap) {
        if (coeff) free(coeff);
        if (tmp)   free(tmp);
        cap = (size_t)max_n + 1;
        coeff = (seq_t *)malloc(cap * sizeof(seq_t));
        tmp   = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!coeff || !tmp) return 0; /* allocation failure, return 0 */
    }
    /* Initialize polynomial: product of empty set is 1 */
    for (idx_t i = 0; i <= max_n; ++i) coeff[i] = 0;
    coeff[0] = 1;

    /* Apply each factor (1 + 2*x^(2^k-1) + 3*x^(2^k)) */
    idx_t power = 2; /* 2^1 */
    while (power <= max_n || power - 1 <= max_n) {
        idx_t a = power - 1;
        idx_t b = power;
        /* tmp initially copies coeff (the multiplier 1) */
        for (idx_t i = 0; i <= max_n; ++i) tmp[i] = coeff[i];
        /* Contribution of 2*x^a */
        for (idx_t i = a; i <= max_n; ++i)
            tmp[i] += (seq_t)2 * coeff[i - a];
        /* Contribution of 3*x^b */
        for (idx_t i = b; i <= max_n; ++i)
            tmp[i] += (seq_t)3 * coeff[i - b];
        /* Prepare for next factor */
        seq_t *swap = coeff;
        coeff = tmp;
        tmp   = swap;
        /* Next power of two */
        if (power > ((idx_t)-1) >> 1) break;   /* avoid overflow */
        power <<= 1;
    }

    /* Compute partial sum up to n */
    seq_t sum = 0;
    for (idx_t i = 0; i <= max_n; ++i) sum += coeff[i];
    return sum;
}
