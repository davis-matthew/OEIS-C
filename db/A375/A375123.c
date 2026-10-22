/*
 * A375123  Weakly increasing run-leader transformation for standard
 *          compositions.
 * https://oeis.org/A375123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the composition obtained by taking the leaders
 *            of the weakly increasing runs of the composition of n in the
 *            graded reverse‑lexicographic (A066099) order
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   comp_to_index, get_composition, get_leaders
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/comp_to_index.h"
#include "../utils/get_composition.h"
#include "../utils/get_leaders.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t comp[64];
    size_t comp_len = 0;
    get_composition(n, comp, &comp_len);

    idx_t leaders[64];
    size_t leaders_len = 0;
    get_leaders(comp, comp_len, leaders, &leaders_len);

    return comp_to_index(leaders, leaders_len);
}
