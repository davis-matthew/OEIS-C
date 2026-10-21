/*
 * A255568  Numbers in whose binary representation there are six 1-bits more
 *          than there are nonleading 0-bits.
 * https://oeis.org/A255568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 3927, u32 n <= 9802, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   _binom_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A255568. 
   The binary representation of a(n) has six more 1‑bits than non‑leading 0‑bits. 
   The algorithm enumerates integers by increasing bit‑length B (even, B ≥ 6). 
   For a given B, the number of valid integers is C(B‑1, (B‑6)/2). 
   Within that block we construct the (rem‑th) integer by choosing the positions 
   of the required zeros in lexicographic (numeric) order. */
#include <stdint.h>
#include "../utils/_binom_uint32.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */
    idx_t rem = n - 1;      /* zero‑based index within sequence */

    uint32_t B = 6;         /* current bit‑length (must be even ≥ 6) */
    while (1) {
        uint32_t zeros = (B - 6) / 2;
        idx_t count = (idx_t)_binom_uint32(B - 1, zeros);
        if (rem < count) break;
        rem -= count;
        B += 2;
    }

    uint32_t zeros = (B - 6) / 2;        /* remaining zeros to place */
    uint32_t total_bits = B - 1;         /* bits after the leading 1 */
    seq_t value = ((seq_t)1 << (B - 1)); /* set the leading 1 */

    for (uint32_t i = 0; i < total_bits; ++i) {
        uint32_t pos = total_bits - 1 - i;          /* actual bit position from LSB */
        if (zeros == 0) {                          /* all remaining bits are 1 */
            for (uint32_t j = i; j < total_bits; ++j) {
                uint32_t p = total_bits - 1 - j;
                value |= ((seq_t)1 << p);
            }
            break;
        }

        idx_t comb = (idx_t)_binom_uint32(total_bits - i - 1, zeros - 1);
        if (rem < comb) {                         /* put a 0 at this position */
            zeros--;                              /* consume one zero */
        } else {                                  /* put a 1 at this position */
            value |= ((seq_t)1 << pos);
            rem -= comb;
        }
    }
    return value;
}
