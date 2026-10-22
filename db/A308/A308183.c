/*
 * A308183  S_oo, where S_1 = bc, S_n = S_{n-1} a^n S_{n-1} for n > 1, over
 *          the alphabet {a,b,c} = {0,1,2}.
 * https://oeis.org/A308183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The infinite word S∞ over {a,b,c} with a=0, b=1, c=2 is built by
 *            S1 = “bc”, S_n = S_{n-1} a^n S_{n-1} for n > 1. a(n) is the nth
 *            symbol (1‑indexed) of S∞
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Return the symbol at position n (1‑indexed).  The result is 0, 1, or 2. */
seq_t
a(idx_t n)
{
    /* Lengths of S_k:  L_1 = 2,   L_k = 2·L_{k-1} + k.  */
    idx_t L[200];
    idx_t k = 1;
    L[1] = 2;                 /* |S₁| = 2  */

    /* Find the smallest k with L_k ≥ n.  k is very small for any reasonable n. */
    while (L[k] < n) {
        ++k;
        /* Compute the next length; use 128‑bit intermediate to avoid overflow. */
        unsigned __int128 tmp = (unsigned __int128)L[k-1] * 2 + (unsigned __int128)k;
        if (tmp > (unsigned __int128)UINT64_MAX) {
            L[k] = (idx_t)UINT64_MAX;
        } else {
            L[k] = (idx_t)tmp;
        }
        if (k >= 199) break;   /* safety against accidental buffer overflow */
    }

    /* Work backwards from level k to find the character. */
    while (k > 1) {
        if (n <= L[k-1]) {
            /* Position is in the left copy of S_{k-1}. */
            k--;
            continue;
        }
        if (n <= L[k-1] + k) {
            /* Position lies in the middle block a^k.  All letters are 'a' (0). */
            return (seq_t)0;
        }
        /* Position is in the right copy of S_{k-1}. Skip left part and middle block. */
        n -= L[k-1] + k;
        k--;
    }

    /* k == 1:  we are inside S₁ = “bc”  (indices 1→'b', 2→'c'). */
    return (seq_t)(n == 1 ? 1 : 2);
}
