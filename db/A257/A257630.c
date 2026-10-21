/*
 * A257630  Near-repdigit triangular numbers.
 * https://oeis.org/A257630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th near‑repdigit triangular number.
   A near‑repdigit is a decimal number whose digits consist of exactly
   two distinct values and where at least one of them occurs exactly
   once. The function iteratively generates triangular numbers
   T_k = k(k+1)/2 and stores those that satisfy the near‑repdigit
   condition.  The k‑th element of the stored list is returned. */
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t filled = 0;          /* number of terms already cached   */
    static seq_t last_tri_val = (seq_t)0; /* T_{last_tri_index}            */
    static idx_t last_tri_index = 0;       /* index of last triangular number */

    if (n == 0) return (seq_t)0; /* undefined for n = 0, just return 0 */

    if (filled < n) {
        idx_t i = last_tri_index;
        seq_t T = last_tri_val;

        for (;;) {
            ++i;
            /* T_i = T_{i-1} + i  (using seq_t arithmetic) */
            T += (seq_t)i;
            last_tri_index = i;
            last_tri_val = T;

            unsigned int freq[10];
            digit_frequencies(T, freq);

            int distinct = 0, onecount = 0;
            for (int d = 0; d < 10; ++d) {
                if (freq[d]) {
                    ++distinct;
                    if (freq[d] == 1) ++onecount;
                }
            }

            if (distinct == 2 && onecount >= 1) {
                if (filled == cache_cap) {
                    size_t newcap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = newcap;
                }
                cache[filled++] = T;
                if (filled == n) break;
            }
        }
    }
    return cache[n - 1];
}
