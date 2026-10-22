/*
 * A354856  a(1) = 1, a(n) = the number of times a(n-1) appears among the
 *          first n-2 terms.
 * https://oeis.org/A354856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of times a(n-1) appears among the first n-2 terms.
 *            Initial values: a(1)=1, a(2)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static storage for the sequence and a frequency table. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;

    static uint64_t *freq = NULL;
    static size_t freq_cap = 0;
    static int initialized = 0;

    /* ---------- Initialization ---------- */
    if (!initialized) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        seq[0] = (seq_t)1;  /* a(1) */
        seq[1] = (seq_t)0;  /* a(2) */
        seq_len = 2;

        freq_cap = 16;
        freq = (uint64_t *)calloc(freq_cap, sizeof(uint64_t));

        /* Count the first term for future queries. */
        uint64_t key = (uint64_t)seq[0];
        if (key >= freq_cap) {
            size_t new_cap = freq_cap;
            while (new_cap <= key) new_cap <<= 1;
            freq = (uint64_t *)realloc(freq, new_cap * sizeof(uint64_t));
            for (size_t i = freq_cap; i < new_cap; ++i) freq[i] = 0;
            freq_cap = new_cap;
        }
        freq[key] = 1;

        initialized = 1;
    }

    /* ---------- Return already known term ---------- */
    if (n <= seq_len)
        return seq[n - 1];

    /* ---------- Grow the sequence array to accommodate n ---------- */
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* ---------- Compute missing terms up to n ---------- */
    for (size_t idx = seq_len; idx < (size_t)n; ++idx) {
        /* Index of the previous term (a(idx)) to look up in freq. */
        uint64_t target = (uint64_t)seq[idx - 1];

        /* Ensure the frequency table can hold the key. */
        if (target >= freq_cap) {
            size_t new_cap = freq_cap;
            while (new_cap <= target) new_cap <<= 1;
            freq = (uint64_t *)realloc(freq, new_cap * sizeof(uint64_t));
            for (size_t i = freq_cap; i < new_cap; ++i) freq[i] = 0;
            freq_cap = new_cap;
        }

        /* a(idx+1) is the current count of the previous value. */
        seq[idx] = (seq_t)freq[target];

        /* After computing a(idx+1), add the previous term to the table
           so that future queries count it. */
        if (target >= freq_cap) {
            size_t new_cap = freq_cap;
            while (new_cap <= target) new_cap <<= 1;
            freq = (uint64_t *)realloc(freq, new_cap * sizeof(uint64_t));
            for (size_t i = freq_cap; i < new_cap; ++i) freq[i] = 0;
            freq_cap = new_cap;
        }
        freq[target] += 1;
    }

    seq_len = n;
    return seq[n - 1];
}
