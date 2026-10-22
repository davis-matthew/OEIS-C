/*
 * A335482  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (2,3,1).
 * https://oeis.org/A335482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute A335482 by brute‑force, examining each integer k,
   building its composition in standard order, and retaining those
   whose parts contain a subsequence with relative order (2,3,1). */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* cached sequence values */
static seq_t *values = NULL;
static size_t values_len = 0;
static size_t values_cap = 0;
static idx_t k_processed = 0;          /* last k that was examined */

/* append a value to the dynamic array */
static void append_value(seq_t v)
{
    if (values_len == values_cap) {
        size_t new_cap = values_cap ? values_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) { EXIT_FAILURE; }
        values = tmp;
        values_cap = new_cap;
    }
    values[values_len++] = v;
}

/* return the n‑th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    while (values_len < n) {
        ++k_processed;                /* next candidate k */

        /* Build the composition of k_processed */
        idx_t temp = k_processed;
        size_t pos[sizeof(idx_t) * 8];
        size_t pos_sz = 0;
        idx_t bit_pos = 1;
        while (temp) {
            if (temp & 1ULL) {
                pos[pos_sz++] = bit_pos;
            }
            ++bit_pos;
            temp >>= 1;
        }

        /* first differences with leading 0 */
        int parts[sizeof(idx_t) * 8];
        size_t part_sz = 0;
        int prev = 0;
        for (size_t i = 0; i < pos_sz; ++i) {
            parts[part_sz++] = (int)(pos[i] - (idx_t)prev);
            prev = pos[i];
        }

        /* reverse the parts to obtain the standard order */
        for (size_t i = 0; i < part_sz / 2; ++i) {
            int t = parts[i];
            parts[i] = parts[part_sz - 1 - i];
            parts[part_sz - 1 - i] = t;
        }

        /* test for pattern (2,3,1): a<b and c<a */
        bool ok = false;
        for (size_t i = 0; i < part_sz && !ok; ++i) {
            int a_val = parts[i];
            for (size_t j = i + 1; j < part_sz && !ok; ++j) {
                int b_val = parts[j];
                for (size_t k = j + 1; k < part_sz; ++k) {
                    int c_val = parts[k];
                    if (c_val < a_val && a_val < b_val) {
                        ok = true;
                        break;
                    }
                }
            }
        }

        if (ok) {
            append_value((seq_t)k_processed);
        }
    }

    return values[n - 1];
}
