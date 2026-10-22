/*
 * A392706  Least even-indexed part in row n of A391983 (the 0-based partial
 *          alternating sums of the n-th composition in standard order).
 * https://oeis.org/A392706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Algorithmic description:
   For a given index n (1‑based), form the nth standard composition via
   the mapping described in A066099:
      1. Write n in binary.
      2. Read the bits from least significant to most significant; the
         positions (1‑based) of 1​'s form a strictly increasing sequence.
      3. Prepend a zero to this sequence, take successive differences,
         and reverse the resulting list.  The reverse list is the
         composition of n.
   Once the composition q = (q1, q2, …, qk) is obtained, compute its
   0‑based partial alternating sums:
      S0 = 0,
      for i = 1..k:  Si = (-1)^i * Σ_{j=1..i} (-1)^j * qj .
   (These Si correspond to the “row” of A391983 for the composition.)
   a(n) is the minimum of the odd‑index (i = 1,3,5,…) partial sums Si.
   The implementation below follows this procedure directly,
   using only integer arithmetic allowed by the build environment.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build the list of 1‑bit positions (1‑based) from n,
       pre‑pend 0, compute differences, and reverse.   */
    unsigned pos[65];          /* pos[0] = 0, then bit positions    */
    unsigned poscnt = 1;       /* start with the leading zero      */
    unsigned i;
    idx_t tmp = n;             /* n >= 1, bit operations are safe */

    for (i = 0; tmp; ++i) {
        if (tmp & 1) {
            pos[poscnt++] = i + 1;   /* bit position + 1 (1‑based)   */
        }
        tmp >>= 1;
    }

    /* Differences between successive positions give composition parts. */
    seq_t diff[65];
    unsigned diffcnt = poscnt - 1;
    unsigned j;
    for (j = 1; j < poscnt; ++j) {
        diff[j - 1] = (seq_t)(pos[j] - pos[j - 1]);  /* strictly positive */
    }

    /* Scan the parts from left to right (reversed diff order),
       computing alternating partial sums and tracking the min at odd
       indices (i = 1,3,5,…).                               */
    bool first = true;
    seq_t best = 0;
    seq_t alt   = 0;          /* current alternating sum Σ(-1)^j qj */

    for (j = 0; j < diffcnt; ++j) {
        unsigned idx = j + 1;                         /* 1‑based part index          */
        seq_t part = diff[diffcnt - 1 - j];           /* reverse the diff order       */

        /* Update the alternating sum: add (-1)^idx * part. */
        if (idx & 1) {                /* odd index → sign = -1       */
            alt -= part;
        } else {                      /* even index → sign = +1      */
            alt += part;
        }

        /* Partial sum Si = (-1)^idx * alt for odd idx. */
        if (idx & 1) {
            seq_t Si = -alt;          /* (-1)^odd = -1              */
            if (first || Si < best) {
                best = Si;
                first = false;
            }
        }
    }

    return best;
}
