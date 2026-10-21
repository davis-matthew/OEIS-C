/*
 * A289611  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 343) or the same sequence
 *          for the mesh patterns (12, 349), (12, 373), (12, 469).
 * https://oeis.org/A289611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - 2^(n-2) - n + 3 for n > 2 a(0) = a(1) = a(2)
 *            = 1 (catalan(·) and big_pow(·,·) are provided by the utils
 *            library)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                    /* for uintptr_t, uint64_t, etc. */
#include "../utils/catalan.h"          /* seq_t catalan(idx_t k);        */
#include "../utils/big_pow.h"          /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* The first three terms are explicitly defined. */
    if (n <= 2)
        return (seq_t)1;

    /* Compute Catalan(n) and 2^(n-2) exactly. */
    seq_t catalan_n   = catalan(n);
    seq_t pow_two     = big_pow((seq_t)2, (seq_t)(n - 2));

    /* a(n) = Catalan(n) - 2^(n-2) - n + 3. */
    seq_t result = catalan_n - pow_two;   /* intermediate difference */
    result -= (seq_t)n;                   /* subtract n   */
    result += (seq_t)3;                   /* add 3       */

    return result;
}
