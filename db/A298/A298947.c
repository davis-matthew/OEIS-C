/*
 * A298947  Number of integer partitions y of n such that exactly one
 *          permutation of y is a Lyndon word.
 * https://oeis.org/A298947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 * + Σ_{d=1}^{n-1} ⌊(n‑1)/(d+1)⌋ * + Σ_{c=2}^{n-1}
 *            (⌊(n‑1)/c⌋ – ⌊n/(c+1)⌋) * + ⌊((n/2)-1)/2⌋ (if n is even) * *
 *            This counts the partitions of n that contain at most two
 *            distinct parts * and for which exactly one permutation yields a
 *            Lyndon word. * * - The first sum counts the partitions w...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are supplied by the build – see the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0)   /* not used, but safe */
        return 0;

    if (cache_size < n) {
        /* extend the cache */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)   /* allocation failure: return 0 (undefined behaviour) */
            return 0;
        cache = tmp;

        /* compute values for indices not yet stored */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            seq_t val = 1;                 /* the single‑part partition */

            idx_t km1 = k - 1;             /* n - 1 */

            /* first sum – c = 1, d = 1 .. k-1 */
            for (idx_t d = 1; d < k; ++d) {
                val += (seq_t)(km1 / (d + 1));
            }

            /* second sum – d = 1, c = 2 .. k-1 */
            for (idx_t c = 2; c < k; ++c) {
                val += (seq_t)((km1 / c) - (k / (c + 1)));
            }

            /* third term – the special case c = d = 2 (only for even k) */
            if ((k & 1) == 0) {
                idx_t half = k >> 1;              /* k / 2 */
                val += (seq_t)((half - 1) >> 1);   /* floor((half-1)/2) */
            }

            cache[k] = val;
        }

        cache_size = n;
    }

    return cache[n];
}
