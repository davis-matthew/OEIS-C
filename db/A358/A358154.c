/*
 * A358154  a(n) is the smallest composite number obtained by appending one or
 *          more 1's to n.
 * https://oeis.org/A358154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A358154: smallest composite obtained by appending
   one or more '1's to the decimal representation of n.
   For each n it tests the three candidates
      10*n + 1,
      100*n + 11,
      1000*n + 111
   and returns the first composite.  The divisibility‑by‑3 argument
   guarantees that one of these is composite. */
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t cand = (seq_t)10  * sn + (seq_t)1;
    if (!isPrimeGeneric(cand))
        return cand;

    cand = (seq_t)100 * sn + (seq_t)11;
    if (!isPrimeGeneric(cand))
        return cand;

    /* the third candidate is guaranteed composite (divisible by 3) */
    return (seq_t)1000 * sn + (seq_t)111;
}
