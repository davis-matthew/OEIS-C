/*
 * A277154  Lexicographically least sequence of nonnegative integers that
 *          avoids 10/7-powers.
 * https://oeis.org/A277154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9948, u16 n <= 9948, u32 n <= 9948, u64 n <= 9948,
 *            u128 n <= 9948, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A277154: lexicographically least nonnegative integer sequence
 * that avoids 10/7-powers.
 *
 * It builds the sequence greedily.  For each index n the smallest
 * non‑negative value a is chosen such that no suffix ending at n
 * forms a 10/7‑power (a block of length 10k = 7k + 3k that ends
 * with the first 3k symbols of the preceding 7k block).  The
 * sequence is memoised so calls in arbitrary order are fast. */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for computed values and a parallel flag array */
    static seq_t   *seq  = NULL;
    static unsigned char *vis  = NULL;
    static size_t   cap   = 0;

    /* ensure capacity for indices up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp_seq) return (seq_t)0; /* allocation failed: return 0 */
        seq = tmp_seq;
        unsigned char *tmp_vis = (unsigned char *)realloc(vis, new_cap * sizeof(unsigned char));
        if (!tmp_vis) return (seq_t)0;
        vis = tmp_vis;
        /* zero the newly allocated part of vis */
        memset(vis + cap, 0, (new_cap - cap) * sizeof(unsigned char));
        cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (vis[(size_t)i]) continue;   /* already computed */
        seq_t val = 0;
        /* try values until a 10/7‑power is avoided */
        while (1) {
            bool has_power = false;
            /* for each possible k such that 10k <= i+1 */
            idx_t k_max = (i + 1) / 10;
            for (idx_t k = 1; k <= k_max && !has_power; ++k) {
                const idx_t start = i + 1 - 10 * k;   /* start index of the 10k block */
                const idx_t root_len = 7 * k;         /* length of the 7k root */
                bool power_ok = true;
                for (idx_t j = 0; j < 3 * k; ++j) {
                    const idx_t left_idx  = start + root_len + j; /* suffix position */
                    const idx_t right_idx = start + j;           /* prefix position */
                    seq_t left_val  = (left_idx == i) ? val : seq[(size_t)left_idx];
                    seq_t right_val = seq[(size_t)right_idx];
                    if (left_val != right_val) {
                        power_ok = false;
                        break;
                    }
                }
                if (power_ok) has_power = true;
            }
            if (!has_power) {                    /* value accepted */
                seq[(size_t)i] = val;
                vis[(size_t)i] = 1;
                break;
            }
            ++val;  /* try next integer */
        }
    }
    return seq[(size_t)n];
}
