/*
 * A371444  Numbers whose binary indices are composite numbers.
 * https://oeis.org/A371444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i>=0; bit_i(n)=1} 2^{c_i-1} where c_i is the i‑th
 *            composite number (c_0 = 4, c_1 = 6, …)
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Count bits of n.  For n>=1 this is the number of positions needed. */
    idx_t tmp = n;
    size_t bits = 0;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    /* Allocate array of the first ‘bits’ composite numbers. */
    idx_t *comps = (idx_t *)malloc(bits * sizeof(idx_t));
    if (!comps) return (seq_t)0;          /* allocation failure → 0 */

    idx_t cand = 4;                       /* first composite number */
    size_t i = 0;
    while (i < bits) {
        /* Skip primes.  (cands 1–3 are not used anyway.) */
        while (is_prime(cand)) ++cand;
        comps[i++] = cand;
        ++cand;
    }

    /* Build the result by mapping bits of n to composite positions. */
    seq_t result = 0;
    size_t bit_ind = 0;
    while (n) {
        if (n & 1) {
            result += pow2((idx_t)(comps[bit_ind] - 1));
        }
        n >>= 1;
        ++bit_ind;
    }

    free(comps);
    return result;
}
