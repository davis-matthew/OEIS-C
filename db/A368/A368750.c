/*
 * A368750  Irregular triangle read by rows: T(n,k) is the number of atoms
 *          contained in the k-th balanced string of left/right parentheses of
 *          length 2*n, where strings within a row are in reverse
 *          lexicographical order.
 * https://oeis.org/A368750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of atoms in the n-th balanced parentheses string
 *            (listed in reverse lexicographic order)
 * Range:     u8 n <= 36, u16 n <= 8063, u32 n <= 8063, u64 n <= 8063,
 *            u128 n <= 8063, big any n
 * Helpers:   small_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/small_binomial.h"

/* Return the number of atoms in the k-th (0‑based) string of length
   2*pair_count in reverse lexicographic order. */
static seq_t count_atoms(idx_t pair_count, idx_t k)
{
    idx_t total_pos = 2 * pair_count;
    idx_t open_rem = pair_count, close_rem = pair_count;
    /* Build the string in an array of chars */
    char *s = (char*)malloc(total_pos);
    if (!s) return 0; /* allocation failure, return 0 as safe value */

    for (idx_t pos = 0; pos < total_pos; ++pos) {
        idx_t rem = total_pos - pos;
        /* Number of strings starting with ')' from this position */
        idx_t cnt_start_right = 0;
        if (close_rem > 0)
            cnt_start_right = small_binomial(rem - 1, open_rem);
        if (k < cnt_start_right) {
            s[pos] = ')';
            --close_rem;
        } else {
            s[pos] = '(';
            k -= cnt_start_right;
            --open_rem;
        }
    }

    /* Count atoms in the constructed string */
    int counter = 0;
    seq_t atoms = 0;
    bool block_start_is_left = false;
    for (idx_t pos = 0; pos < total_pos; ++pos) {
        char ch = s[pos];
        if (counter == 0)
            block_start_is_left = (ch == '(');
        if (ch == '(')
            ++counter;
        else
            --counter;
        if (counter == 0 && block_start_is_left)
            ++atoms;
    }

    free(s);
    return atoms;
}

/* Main sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1, not used but safe */

    /* Find the row 'row' such that cumulative count up to this row
       contains the n-th term. */
    idx_t row = 1;
    idx_t cumulative = 0;
    while (1) {
        idx_t row_len = small_binomial(2 * row, row);
        if (cumulative + row_len >= n)
            break;
        cumulative += row_len;
        ++row;
    }

    /* Position within this row (0‑based) */
    idx_t k = n - cumulative - 1;

    return count_atoms(row, k);
}
