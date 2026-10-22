/*
 * A357134  Take the k-th composition in standard order for each part k of the
 *          n-th composition in standard order; then set a(n) to be the index
 *          (in standard order) of the concatenation.
 * https://oeis.org/A357134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   composition_from_index, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in OEIS A357134:  
   For the nth composition C = (c1,…,cm) in standard order, take the
   ci‑th composition for each part, concatenate all these compositions,
   and return the index (in standard order) of the resulting composition. */

#include <stddef.h>
#include "../utils/seq_pow.h"
#include "../utils/composition_from_index.h"

/* Compute the index (in standard order) of a composition given by parts[]. */
static seq_t
index_of_composition(const idx_t *parts, size_t len)
{
    if (len == 0)
        return (seq_t)0;

    seq_t result = 0;
    /* positions of 1 bits are cumulative sums of parts in reverse order */
    idx_t sum = 0;
    for (idx_t i = len; i > 0; --i) {
        sum += parts[i - 1];
        /* add 2^(sum-1) to result */
        result += seq_pow((seq_t)2, (idx_t)(sum - 1));
    }
    return result;
}

/* Main function: returns a(n) for OEIS sequence A357134. */
seq_t a(idx_t n)
{
    /* obtain the n-th composition */
    idx_t parts_n[64];
    size_t len_n;
    composition_from_index(n, parts_n, &len_n);

    if (len_n == 0)          /* empty composition for n == 0 */
        return (seq_t)0;

    /* concatenate the compositions of each part */
    idx_t conc[512];          /* enough for n up to ~10k */
    size_t conc_len = 0;

    for (size_t i = 0; i < len_n; ++i) {
        idx_t part = parts_n[i];
        idx_t part_parts[64];
        size_t part_len;
        composition_from_index(part, part_parts, &part_len);

        /* copy this part's composition into the concatenated array */
        for (size_t j = 0; j < part_len; ++j)
            conc[conc_len++] = part_parts[j];
    }

    /* compute the index of the concatenated composition */
    return index_of_composition(conc, conc_len);
}
