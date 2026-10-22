/*
 * A326947  BII-numbers of T_0 set-systems.
 * https://oeis.org/A326947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th BII‑number of a T₀ set‑system
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   BII_is_T0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 3, … */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/BII_is_T0.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used, but safe */

    /* static cache for already generated terms */
    static idx_t seq_len = 0;             /* number of cached terms */
    static idx_t seq_cap = 0;             /* capacity of the array */
    static seq_t *seq = NULL;             /* cached sequence */
    static idx_t next_candidate = 0;      /* next BII-number to test */

    while (seq_len < n) {
        while (true) {
            if (BII_is_T0(next_candidate)) {
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap == 0 ? 1 : seq_cap << 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len] = (seq_t)next_candidate;
                seq_len++;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return seq[n - 1];
}
