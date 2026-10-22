/*
 * A396225  a(n) = (n!)^6.
 * https://oeis.org/A396225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/*
 * Computes a(n) = (n!)^6.
 *
 * Uses a static cache of factorials so that any order of calls is
 * efficient.  The cache is grown only when necessary.
 *
 * All arithmetic is done with the operators allowed for seq_t.
 */
seq_t a(idx_t n)
{
    /* static cache of factorials: fac[i] = i! */
    static seq_t *fac = NULL;
    static size_t fac_cap = 0;   /* capacity of fac[]  */
    static idx_t  fac_max = 0;   /* largest index with known factorial */

    /* initial allocation */
    if (fac == NULL) {
        fac_cap = 1;
        fac = (seq_t *)malloc(fac_cap * sizeof(seq_t));
        if (fac == NULL) return (seq_t)0;   /* allocation failure */
        fac[0] = (seq_t)1;                  /* 0! = 1 */
        fac_max = 0;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > fac_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        fac = tmp;
        fac_cap = new_cap;
    }

    /* extend factorial table up to n */
    for (idx_t i = fac_max + 1; i <= n; ++i) {
        fac[i] = fac[i - 1] * (seq_t)i;
    }
    fac_max = n;

    /* compute (n!)^6 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int k = 0; k < 6; ++k) {
        result *= fac[n];
    }
    return result;
}
