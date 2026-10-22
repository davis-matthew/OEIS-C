/*
 * A386701  a(n) = Sum_{k=0..n} (-3)^(n-k) * binomial(4*n,k).
 * https://oeis.org/A386701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} (-3)^(n‑k) * binomial(4*n, k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t is defined in the build; as are idx_t and any big‑integer features. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached = 0;

    if ((size_t)n >= cached) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;

        for (size_t i = cached; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = 1;
            } else {
                /* Choose the computation path depending on the width of seq_t */
                if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                    /* -------------------------------------------------------- */
                    /* Small width (≤64 bits).  Use 128‑bit intermediates.  */
                    /* -------------------------------------------------------- */
                    unsigned __int128 binom = 1;
                    unsigned __int128 pow3  = 1;
                    for (idx_t j = 0; j < (idx_t)i; ++j) pow3 *= 3;

                    unsigned __int128 pos = 0, neg = 0;

                    for (idx_t k = 0; k <= (idx_t)i; ++k) {
                        unsigned __int128 term = binom * pow3;
                        if (((i - k) & 1) != 0) neg += term;
                        else                       pos += term;

                        if (k < (idx_t)i) {
                            unsigned __int128 mult = (unsigned __int128)(4 * i - k);
                            binom = binom * mult / (unsigned __int128)(k + 1);
                        }
                        pow3 /= 3;
                    }
                    cache[i] = (seq_t)(pos - neg);
                } else {
                    /* -------------------------------------------------------- */
                    /* Large or arbitrary width.  Use seq_t operations.  */
                    /* -------------------------------------------------------- */
                    seq_t binom = 1;
                    seq_t pow3  = 1;
                    for (idx_t j = 0; j < (idx_t)i; ++j) pow3 *= (seq_t)3;

                    seq_t pos = 0;
                    seq_t neg = 0;

                    for (idx_t k = 0; k <= (idx_t)i; ++k) {
                        seq_t term = binom * pow3;
                        if (((i - k) & 1) == 0) pos += term;
                        else                    neg += term;

                        if (k < (idx_t)i) {
                            seq_t mult = (seq_t)(4 * i - k);
                            binom = (binom * mult) / (seq_t)(k + 1);
                        }
                        pow3 /= (seq_t)3;
                    }
                    cache[i] = pos - neg;
                }
            }
        }
        cached = newsize;
    }
    return cache[(size_t)n];
}
