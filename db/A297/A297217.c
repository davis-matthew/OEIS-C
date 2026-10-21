/*
 * A297217  Most common value of the number of divisors function among all
 *          composites up to composite(n) inclusive, or 0 if there is a tie.
 * https://oeis.org/A297217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the most common value of the number of divisors function
 *            among all composite numbers up to the n-th composite number
 *            inclusive. If there is a tie for the most frequent divisor
 *            count, a(n) = 0
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   divisor_count_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, but guard anyway */

    /* static state to keep previous composites */
    static uint64_t next_candidate = 4;      /* next integer to test */
    static idx_t composite_count = 0;        /* number of composites generated so far */
    static seq_t *div_counts = NULL;        /* divisor count for each composite */
    static size_t div_cap = 0;               /* allocated capacity of div_counts */
    static uint32_t global_max_d = 0;        /* largest divisor count seen so far */

    /* generate composites up to the required n */
    if (composite_count < n) {
        /* Ensure enough storage for new composites */
        size_t new_cap = div_cap ? div_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        if (new_cap > div_cap) {
            div_counts = (seq_t *)realloc(div_counts, new_cap * sizeof(seq_t));
            div_cap = new_cap;
        }

        /* find the remaining composites */
        while (composite_count < n) {
            while (is_prime_small((seq_t)next_candidate)) ++next_candidate;  /* skip primes */
            seq_t d = divisor_count_u64(next_candidate);                   /* divisor count */
            div_counts[composite_count++] = d;
            if (d > global_max_d) global_max_d = (uint32_t)d;
            ++next_candidate;                                            /* advance to next integer */
        }
    }

    /* count frequencies of divisor counts among the first n composites */
    int *freq = (int *)calloc(global_max_d + 1, sizeof(int));
    for (idx_t i = 0; i < n; ++i)
        ++freq[(int)div_counts[i]];

    /* find the most frequent divisor count, check for ties */
    int maxfreq = 0, most = 0;
    bool tie = false;
    for (int d = 1; d <= (int)global_max_d; ++d) {
        if (freq[d] > maxfreq) {
            maxfreq = freq[d];
            most = d;
            tie    = false;
        } else if (freq[d] == maxfreq && freq[d] > 0) {
            tie = true;
        }
    }

    free(freq);
    return tie ? (seq_t)0 : (seq_t)most;
}
