/*
 * A395276  a(n) = (2*n)! * [x^(2*n)] cos(x) * cos(sqrt(3)*x) / cos(3*x).
 * https://oeis.org/A395276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)! * [x^(2*n)] cos(x) * cos(sqrt(3)*x) / cos(3*x)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (-1)^n * B(n) - sum_{k=0}^{n-1} (-9)^(n-k) * binomial(2*n,2*k) * a(k) */
/* where B(n) = sum_{k=0}^n binomial(2*n,2*k) * 3^k */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static idx_t seq_last = (idx_t)-1;   /* last computed index */

    /* power of 3 and 9 */
    static seq_t *pow3 = NULL;
    static seq_t *pow9 = NULL;
    static size_t pow_cap = 0;

    /* initialize cache lazily */
    if (seq_last == (idx_t)-1) {
        seq_cap = 1;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0;          /* insufficient memory; should not happen */
        seq[0] = 1;                  /* a(0) = 1 */
        seq_last = 0;

        pow_cap = 1;
        pow3 = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        pow9 = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        if (!pow3 || !pow9) return 0;
        pow3[0] = 1;
        pow9[0] = 1;
    }

    /* resize arrays if needed */
    if (n >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    if (n >= pow_cap) {
        size_t new_cap = pow_cap ? pow_cap * 2 : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        pow3 = (seq_t*)realloc(pow3, new_cap * sizeof(seq_t));
        pow9 = (seq_t*)realloc(pow9, new_cap * sizeof(seq_t));
        pow_cap = new_cap;
    }

    /* extend powers of 3 and 9 */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        pow3[i] = seq_mul(pow3[i - 1], (seq_t)3);
        pow9[i] = seq_mul(pow9[i - 1], (seq_t)9);
    }

    /* compute sequence values up to n */
    for (idx_t m = seq_last + 1; m <= n; ++m) {
        /* compute B(m) = sum_{k=0}^{m} binom(2m,2k) * 3^k */
        seq_t B = 0;
        for (idx_t k = 0; k <= m; ++k) {
            seq_t bin = binomial((idx_t)(2 * m), (idx_t)(2 * k));
            seq_t term = seq_mul(bin, pow3[k]);
            B += term;
        }

        /* compute S(m) = sum_{k=0}^{m-1} (-9)^{m-k} * binom(2m,2k) * a(k) */
        seq_t posS = 0, negS = 0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t bin = binomial((idx_t)(2 * m), (idx_t)(2 * k));
            seq_t term = seq_mul(bin, seq[k]);          /* bin * a(k) */
            term = seq_mul(term, pow9[m - k]);          /* *|(-9)^{m-k}| */
            if ((m - k) % 2 == 0) {
                posS += term;
            } else {
                negS += term;
            }
        }

        /* compute a(m) */
        if ((m & 1) == 0) { /* m even: a = (B + negS) - posS */
            seq_t temp = B + negS;
            seq[m] = temp - posS;
        } else {           /* m odd: a = negS - (posS + B) */
            seq_t temp = posS + B;
            seq[m] = negS - temp;
        }
    }
    seq_last = n;
    return seq[n];
}
