/*
 * A334510  Eventual period of a single cell in rule 73 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   equal_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Definition: a(n) is the eventual period of the elementary cellular automaton
   with rule 73 started from a single active cell on a cyclic lattice of width n.
   Computed by simulating the automaton and using Floyd's cycle‑finding algorithm.
*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/equal_state.h"

#define MAX_N 20000                     /* safe upper bound for n (≈10000) */
#define MAX_W ((MAX_N + 63) / 64)       /* maximum number of 64‑bit words needed */

static inline void step_once(const uint64_t *in, uint64_t *out,
                             size_t w, uint64_t mask_last, uint64_t bit_last)
{
    const uint64_t ALL_ONES = ~0ULL;
    if (w == 1) {
        uint64_t value = in[0];
        uint64_t L = ((value << 1) | (value >> bit_last)) & mask_last;
        uint64_t R = ((value >> 1) | (value << bit_last)) & mask_last;
        out[0] = ALL_ONES ^ L ^ value ^ R ^ (L & R) ^ (L & value & R);
        out[0] &= mask_last;
        return;
    }

    /* temporary storage for left and right neighbor vectors */
    static uint64_t L[MAX_W];
    static uint64_t R[MAX_W];

    /* compute left neighbors */
    uint64_t carry_left = (in[w-1] >> bit_last) & 1;
    L[0] = (in[0] << 1) | carry_left;
    for (size_t i = 1; i < w; ++i)
        L[i] = (in[i] << 1) | ((in[i-1] >> 63) & 1);

    /* compute right neighbors */
    uint64_t carry_right = in[0] & 1;
    R[w-1] = (in[w-1] >> 1) | (carry_right << 63);
    for (size_t i = w-1; i-- > 0; )
        R[i] = (in[i] >> 1) | ((in[i+1] & 1) << 63);

    /* produce next state */
    for (size_t i = 0; i < w; ++i)
        out[i] = ALL_ONES ^ L[i] ^ in[i] ^ R[i] ^ (L[i] & R[i]) ^ (L[i] & in[i] & R[i]);

    out[w-1] &= mask_last;         /* zero unused bits */
}

seq_t a(idx_t n)
{
    size_t w = (n + 63) / 64;
    uint64_t mask_last = (n & 63) == 0 ? ~0ULL : ((1ULL << (n & 63)) - 1);
    uint64_t bit_last = (n - 1) & 63;

    uint64_t init[MAX_W] = {0};
    uint64_t tort[MAX_W];
    uint64_t hare[MAX_W];
    uint64_t tmp[MAX_W];

    init[0] = 1;
    init[0] &= mask_last;                /* safety for n=1 */

    /* first step for tortoise, two steps for hare */
    step_once(init, tort, w, mask_last, bit_last);
    step_once(tort, hare, w, mask_last, bit_last);
    step_once(hare, hare, w, mask_last, bit_last);

    /* Floyd's cycle detection */
    while (!equal_state(tort, hare, w)) {
        step_once(tort, tmp, w, mask_last, bit_last);
        memcpy(tort, tmp, w * sizeof(uint64_t));

        step_once(hare, tmp, w, mask_last, bit_last);
        memcpy(hare, tmp, w * sizeof(uint64_t));
        step_once(hare, tmp, w, mask_last, bit_last);
        memcpy(hare, tmp, w * sizeof(uint64_t));
    }

    /* find cycle length (lambda) */
    size_t lambda = 1;
    step_once(tort, hare, w, mask_last, bit_last);
    while (!equal_state(tort, hare, w)) {
        step_once(hare, tmp, w, mask_last, bit_last);
        memcpy(hare, tmp, w * sizeof(uint64_t));
        ++lambda;
    }

    return (seq_t)lambda;
}
