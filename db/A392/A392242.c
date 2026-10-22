/*
 * A392242  Squares with distinct digits.
 * https://oeis.org/A392242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) = square of the (n-1)th integer whose square has distinct
 *            digits
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 566, u64 n <= 611,
 *            u128 n <= 611, big any n
 * Helpers:   digit_counts, ensure_capacity, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_counts.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t term_count = 0;
    static idx_t next_candidate = 0;

    while (term_count < n) {
        seq_t base = (seq_t)next_candidate;
        seq_t sq   = seq_mul(base, base);

        unsigned char freq[10] = {0};
        digit_counts(sq, freq);

        bool distinct = true;
        for (int d = 0; d < 10; ++d) {
            if (freq[d] > 1) {
                distinct = false;
                break;
            }
        }

        if (distinct) {
            if (cap <= term_count) {
                ensure_capacity(term_count + 1, &cap, (void **)&cache, sizeof(seq_t));
            }
            cache[term_count++] = sq;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
