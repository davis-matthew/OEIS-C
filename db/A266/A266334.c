/*
 * A266334  G.f. = b(2)*b(6)*b(10)/(x^14+x^12-x^5-x^3-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. = b(2)*b(6)*b(10)/(x^14+x^12-x^5-x^3-x+1), where b(k) =
 *            (1-x^k)/(1-x)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 45, u64 n <= 94, u128 n <= 194,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int Q[18] = {
    /*  q0 ... q17  */
    1,  -4,   6,  -5,   4,  -4,   4,  -3,   1,
     0,   0,   0,   1,  -3,   4,  -4,   3,  -1
};

static const int P[19] = {
    /*  P0 ... P18  */
    1, 0, -1, 0, 0, 0,
   -1, 0,  1, 0, -1, 0,
    1, 0, 0, 0,  1, 0, -1
};

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* grow cache if necessary */
    if (cache_size <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;

            /* contributions from Q */
            for (int k = 1; k < 18; ++k) {
                if (i >= (idx_t)k) {
                    int coeff = -Q[k];                /* c = -q_k */
                    if (coeff > 0)
                        pos += cache[i - k] * (seq_t)coeff;
                    else if (coeff < 0)
                        neg += cache[i - k] * (seq_t)(-coeff);
                }
            }

            /* contributions from P (numerator) */
            if (i <= 18) {
                int coeffP = P[i];
                if (coeffP > 0)
                    pos += (seq_t)coeffP;
                else if (coeffP < 0)
                    neg += (seq_t)(-coeffP);
            }

            /* a[i] = pos - neg  (pos >= neg for this sequence) */
            cache[i] = pos - neg;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
