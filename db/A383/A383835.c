/*
 * A383835  Number of permutations of [n] whose compositional square is the
 *          identity permutation or its reverse.
 * https://oeis.org/A383835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000085(n) + A037224(n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   involutions, reverse_root_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000085(n) (involutions) satisfies:            */
/*   a(0)=1, a(1)=1, a(n)=a(n-1)+(n-1)*a(n-2)       */
/* A037224(n) counts permutations whose square is the reverse permutation. */
/* For n % 4 == 2 or 3 there are none; for n % 4 == 1 it equals the value for n-1; */
/* for n % 4 == 0: A037224(n) = 2 * Π_{j=1}^{(n-4)/4} (4*j + 2) */

/* See comments in the code for details. */

#include <stddef.h>
#include "../utils/involutions.h"
#include "../utils/reverse_root_counts.h"

seq_t a(idx_t n) {
    seq_t inv = involutions(n);
    seq_t rev_root = reverse_root_counts(n);
    return inv + rev_root;
}
