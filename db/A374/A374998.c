/*
 * A374998  Position of the last requested element when the elements of the
 *          n-th composition (in standard order) are requested from a
 *          self-organizing list initialized to (1, 2, 3, ...), using the
 *          transpose updating strategy.
 * https://oeis.org/A374998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gen_comp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A374998
 *  a(n) = position of the last requested element when the parts of the n‑th
 *  composition (in standard order) are requested from a self‑organizing list
 *  initialized to (1, 2, 3, …), using the transpose update strategy.
 */
#include <stddef.h>
#include "../utils/gen_comp.h"

seq_t a(idx_t n)
{
    /* find the size of the composition (sum of its parts) */
    idx_t sum = 1;
    idx_t prev_total = 0;
    idx_t block = 1;                 /* number of compositions of current sum */
    while (n > prev_total + block) {
        ++sum;
        prev_total += block;
        block <<= 1;
    }
    idx_t k = n - prev_total;        /* 1‑based index within this block */

    /* generate the k‑th composition of sum 'sum' */
    idx_t comp[64];
    idx_t comp_len;
    gen_comp(sum, k, comp, &comp_len);

    /* simulate the self‑organizing list */
    idx_t list[64];
    for (idx_t i = 0; i < sum; ++i)
        list[i] = i + 1;             /* initial list 1,2,3,… */

    idx_t last_pos = 0;              /* 1‑based position of the last part */
    for (idx_t i = 0; i < comp_len; ++i) {
        idx_t part = comp[i];
        idx_t pos = 0;
        while (list[pos] != part) ++pos;
        last_pos = pos + 1;
        if (pos > 0) {
            idx_t tmp = list[pos];
            list[pos] = list[pos - 1];
            list[pos - 1] = tmp;
        }
    }
    return (seq_t)last_pos;
}
