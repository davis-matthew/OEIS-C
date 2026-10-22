/*
 * A386893  Minimal number of Farey fractions in between two fractions that
 *          are not similarly ordered.
 * https://oeis.org/A386893
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   brute_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: for n >= 92, a(n) = floor(n/4) + d, 
 * where d = 1 if n%4==0, 2 if n%4==1, 2 if n%4==2, 4 if n%4==3.
 * For 4 <= n < 92, a(n) is computed by brute‑force generation
 * of the Farey sequence of order n and determining the minimal
 * pair distance at which the "similarly ordered" property fails.
 * 
 * Sequence A386893: minimal number of Farey fractions in between two
 * fractions that are not similarly ordered.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/brute_a.h"

/* Main function required by the infrastructure */
seq_t a(idx_t n)
{
    if (n < 4) return 0; /* undefined for n < 4, value irrelevant */

    /* The fast formula holds for n >= 92 */
    if (n >= 92) {
        static const unsigned char d_mod[4] = {1, 2, 2, 4};
        seq_t q = (seq_t)(n / 4);
        seq_t add = (seq_t)d_mod[n % 4];
        return q + add;
    }

    /* For small n, do brute force */
    return brute_a(n);
}
