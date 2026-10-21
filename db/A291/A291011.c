/*
 * A291011  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - S)^2 * (1 - 2*S).
 * https://oeis.org/A291011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*3^n - 2^(n-1)*(8+n) for n >= 1 a(0) = 4 Uses
 *            precomputed power tables to keep each call O(1) after the first
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache for powers of 3 and 2 so we only compute each power once */
    static seq_t *pow3 = NULL;
    static seq_t *pow2 = NULL;
    static idx_t  max_pow = (idx_t)-1;   /* highest n for which powers are known */

    /* Ensure we have at least n entries in the power tables */
    if (max_pow != (idx_t)-1 && n > max_pow) {
        /* grow the tables to at least n */
        idx_t new_cap = (max_pow + 1) * 2;
        if (new_cap <= n) new_cap = n + 1;

        pow3 = (seq_t *)realloc(pow3, sizeof(seq_t) * new_cap);
        if (!pow3) abort();
        pow2 = (seq_t *)realloc(pow2, sizeof(seq_t) * new_cap);
        if (!pow2) abort();
    }
    else if (max_pow == (idx_t)-1) {
        /* first time: allocate and init with base cases pow3[0] = 1, pow2[0] = 1 */
        idx_t init_cap = (n + 1) < 4 ? 4 : n + 1;
        pow3 = (seq_t *)realloc(pow3, sizeof(seq_t) * init_cap);
        if (!pow3) abort();
        pow2 = (seq_t *)realloc(pow2, sizeof(seq_t) * init_cap);
        if (!pow2) abort();
        pow3[0] = (seq_t)1;
        pow2[0] = (seq_t)1;
        max_pow = 0;
    }

    /* compute any missing powers up to n */
    for (idx_t i = max_pow + 1; i <= n; ++i) {
        pow3[i] = pow3[i-1] * (seq_t)3;
        pow2[i] = pow2[i-1] * (seq_t)2;
    }
    max_pow = n;

    /* Handle the offset n = 0 separately */
    if (n == 0) return (seq_t)4;

    /* a(n) = 8*3^n - 2^(n-1)*(8+n) */
    seq_t term1 = (seq_t)8 * pow3[n];
    seq_t term2 = pow2[n-1] * ((seq_t)8 + (seq_t)n);
    return term1 - term2;
}
