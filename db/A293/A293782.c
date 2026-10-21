/*
 * A293782  a(n) is the number of permutations of {1, 2, 3, 4} that avoid the
 *          list of pairs encrypted in n (see comments below).
 * https://oeis.org/A293782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   pair_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A293782:  
   a(n) = number of permutations of {1,2,3,4} that avoid
   all ordered pairs indicated by the 12 least‑significant
   bits of n (bit 0 → (1,2), bit 1 → (1,3), …, bit 11 → (4,3)). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pair_bit.h"

/* Permutations of {1,2,3,4} – there are exactly 24 of them. */
/* For each permutation compute a 12‑bit mask: bit i set iff the
   ith adjacent pair equals the pair corresponding to bit i. */
static uint16_t perm_masks[24];
static bool perm_init = false;

/* Recursive helper to enumerate all permutations and fill perm_masks. */
static void generate_perm_masks(int depth, int *perm, bool *used, int *idx)
{
    if (depth == 4) {
        uint16_t m = 0;
        for (int i = 0; i < 3; ++i) {
            int bit = pair_bit(perm[i], perm[i+1]);
            m |= (uint16_t)(1u << bit);
        }
        perm_masks[*idx] = m;
        (*idx)++;
        return;
    }
    for (int v = 1; v <= 4; ++v) {
        if (!used[v-1]) {
            used[v-1] = true;
            perm[depth] = v;
            generate_perm_masks(depth+1, perm, used, idx);
            used[v-1] = false;
        }
    }
}

static void init_perm_masks(void)
{
    int perm[4];
    bool used[4] = {false,false,false,false};
    int idx = 0;
    generate_perm_masks(0, perm, used, &idx);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!perm_init) {
        init_perm_masks();
        perm_init = true;
    }
    uint16_t forbidden = (uint16_t)(n & 0xFFFu);  /* lowest 12 bits */
    int count = 0;
    for (int i = 0; i < 24; ++i) {
        if ((perm_masks[i] & forbidden) == 0) ++count;
    }
    return (seq_t)count;
}
