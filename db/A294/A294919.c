/*
 * A294919  Numbers n such that 2^(n-1), (2*n-1)*(2^((n-1)/2)),
 *          (4*ceiling((1/4)*n)-2), and (2^((n+1)/2) +
 *          floor((3/4)*n)*2^(((n+1)/2)+1)) are all congruent to 1 (mod n).
 * https://oeis.org/A294919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers n such that
   n ≡ 1 (mod 4) and 2^(n‑1) ≡ 1 (mod n) and 2^((n‑1)/2) ≡ –1 (mod n). */
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Check whether a candidate n satisfies the three congruences. */
static bool good_candidate(idx_t cand)
{
    seq_t mod  = cand;          /* modulus is the candidate itself   */
    seq_t two  = (seq_t)2;      /* base 2 as seq_t                     */

    /* 2^(cand‑1) ≡ 1 (mod cand)  */
    if (mod_pow(two, cand-1, mod) != (seq_t)1)
        return false;

    /* 2^((cand‑1)/2) ≡ cand‑1 (≡ –1) (mod cand) */
    if (mod_pow(two, (cand-1)/2, mod) != mod-1)
        return false;

    return true;
}

/* a(n) : nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t cand  = 5;            /* first candidate that can satisfy conditions */

    while (count < n) {
        if (good_candidate(cand)) {
            count++;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 4;              /* only numbers ≡ 1 (mod 4) can work */
    }
    /* Should never reach here for valid inputs */
    return 0;
}
