/*
 * A336683  Sum of 2^k for all residues k found in the Fibonacci sequence mod
 *          n.
 * https://oeis.org/A336683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum(2^k) for all k that appear in the Fibonacci sequence
 *            modulo n. The algorithm: iterate the Fibonacci residue sequence
 *            until it repeats the starting pair (0,1), marking all residues
 *            seen, and accumulate 2^k for each unique residue. The Pisano
 *            period is at most 6n, so the procedure is O(n...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For modulus 1 the only residue is 0; return 2^0 = 1. */
    if (n == 1) return 1;

    /* Allocate a marker array for residues 0..n-1. */
    unsigned char *seen = (unsigned char *)calloc((size_t)n, sizeof(unsigned char));
    if (!seen) return 0;                 /* out of memory: return 0 as a safe fallback */

    seq_t sum = 0;
    idx_t cur = 0;
    idx_t next = 1 % n;  /* 1 mod n is 1 for n>1 */

    /* Run until the sequence returns to (0,1). */
    do
    {
        if (!seen[cur])
        {
            seen[cur] = 1;
            sum += pow2(cur);   /* compute 2^cur and add */
        }
        idx_t tmp = cur + next;
        if (tmp >= n)
            tmp -= n;           /* (cur + next) mod n */
        cur = next;
        next = tmp;
    } while (!(cur == 0 && next == 1));

    free(seen);
    return sum;
}
