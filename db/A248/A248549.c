/*
 * A248549  Numbers n such that the smallest prime divisor of n^2+1 is 61.
 * https://oeis.org/A248549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 389, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248549:
   a(n) = the nth positive integer n such that the smallest prime dividing
   n^2+1 is 61.  Numbers satisfy n ≡ 50 or 72 (mod 122) and avoid
   all smaller primes 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59
   dividing n^2+1.  This implementation generates candidates in ascending
   order and checks the divisibility condition.  It runs in O(n) time
   with little memory usage.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime list smaller than 61 that must not divide n^2+1 */
static const int SMALL_PRIMES[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59
};
static const size_t SMALL_PRIMES_COUNT = sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);

/* Offsets giving the two residue classes modulo 122 that satisfy
   n^2 ≡ -1 (mod 61).  They are 50 and 72.  */
static const int OFFSETS[2] = { 50, 72 };

/* Return the nth term of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    /* Validate input (failing silently is acceptable for the harness). */
    if (n == 0) return (seq_t)0;

    idx_t count = 0;        /* how many terms we have found */
    idx_t m = 0;            /* current block index */
    int offset_idx = 0;     /* 0 or 1 to walk through OFFSETS */

    seq_t result = (seq_t)0;

    while (count < n) {
        /* Generate the next candidate using the pattern 122*m + OFFSETS */
        idx_t candidate = 122 * m + OFFSETS[offset_idx];

        /* Move to the next candidate */
        offset_idx++;
        if (offset_idx == 2) {
            offset_idx = 0;
            m++;
        }

        /* Convert to seq_t for modular arithmetic */
        seq_t cand = (seq_t)candidate;

        /* Check that 61 divides n^2+1 */
        seq_t r61 = cand % 61;
        if ( ((r61 * r61 + 1) % 61) != 0 )
            continue;   /* Should never happen for these residues, but guard */

        /* Reject if any smaller prime divides n^2+1 */
        bool bad = false;
        for (size_t i = 0; i < SMALL_PRIMES_COUNT; ++i) {
            int p = SMALL_PRIMES[i];
            seq_t r = cand % p;
            if ( ((r * r + 1) % p) == 0 ) {
                bad = true;
                break;
            }
        }
        if (bad)
            continue;

        /* Accept the candidate */
        result = cand;
        ++count;
    }

    return result;
}
