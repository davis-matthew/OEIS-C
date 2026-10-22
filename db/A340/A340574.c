/*
 * A340574  a(1) = 112. a(n) is the smallest number k with the sum of the even
 *          digits equal to the sum of the odd digits (A036301), which is not
 *          an earlier term, for which k*a(n - 1) has the sum of the even
 *          digits equal to the sum of the odd digits (A036301).
 * https://oeis.org/A340574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_frequencies, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A340574
 * a(1) = 112.
 * For n > 1 a(n) is the smallest integer k that satisfies:
 *   1) k has equal sums of its even and odd decimal digits
 *      (property of A036301),
 *   2) k has not appeared in a(1)…a(n‑1),
 *   3) the product k * a(n-1) also has equal sums of even and odd digits.
 * The sequence is infinite.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/digit_frequencies.h"

static bool equal_even_odd(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    int even_sum = 0;
    int odd_sum  = 0;
    for (int d = 0; d < 10; ++d) {
        if ((d & 1) == 0) {
            even_sum += freq[d] * d;
        } else {
            odd_sum += freq[d] * d;
        }
    }
    return even_sum == odd_sum;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined for offset 1; safe guard */
    }

    /* static cache of computed terms a(1)...a(len) */
    static seq_t *cache = NULL;
    static size_t len  = 0;
    static size_t cap  = 0;

    /* Allocate or enlarge cache as needed */
    if (cap < (size_t)n) {
        size_t new_cap = (size_t)n;
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
        } else {
            cache = seq_realloc(cache, cap, new_cap);
        }
        cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for ( ; len < (size_t)n; ++len ) {
        if (len == 0) {          /* a(1) = 112 */
            cache[0] = (seq_t)112;
            continue;
        }

        seq_t prev = cache[len - 1];
        seq_t candidate = (seq_t)1; /* start from 1 and search upward */

        for ( ; ; ++candidate ) {
            /* 1) candidate must be in A036301 */
            if (!equal_even_odd(candidate)) {
                continue;
            }

            /* 2) candidate must be unique among previously found terms */
            bool seen = false;
            for (size_t i = 0; i < len; ++i) {
                if (cache[i] == candidate) {
                    seen = true;
                    break;
                }
            }
            if (seen) {
                continue;
            }

            /* 3) product with previous term must also have equal sums */
            seq_t prod = candidate * prev;
            if (equal_even_odd(prod)) {
                cache[len] = candidate;
                break;
            }
        }
    }

    return cache[n - 1];
}
