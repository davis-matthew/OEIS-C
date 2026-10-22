/*
 * A303095  Balanced primes of order one ending in 9.
 * https://oeis.org/A303095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute nth balanced prime of order one ending in 9.
   A prime p is balanced of order one if p = (prev + next)/2,
   where prev and next are the immediate predecessor and successor primes.
   Only primes with p % 10 == 9 are returned.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* external types supplied by the build */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. */
    if (n == 0) return 0; /* defined for safety */

    /* ------------------------------------------------------------------
       Small prime list (up to 500 000) generated once.
       ------------------------------------------------------------------ */
    static uint32_t *smallPrimes = NULL;
    static size_t smallPrimeCount = 0;

    if (smallPrimes == NULL) {
        const uint32_t LIM = 500000;
        bool *mark = (bool *)calloc(LIM + 1, sizeof(bool));
        if (!mark) return 0;          /* memory issue */

        for (uint32_t i = 2; i * i <= LIM; ++i)
            if (!mark[i])
                for (uint32_t j = i * i; j <= LIM; j += i)
                    mark[j] = true;

        for (uint32_t i = 2; i <= LIM; ++i)
            if (!mark[i]) ++smallPrimeCount;

        smallPrimes = (uint32_t *)calloc(smallPrimeCount, sizeof(uint32_t));
        if (!smallPrimes) {
            free(mark);
            return 0;
        }
        size_t idx = 0;
        for (uint32_t i = 2; i <= LIM; ++i)
            if (!mark[i]) smallPrimes[idx++] = i;

        free(mark);
    }

    /* ------------------------------------------------------------------
       Prime searching with a sliding window.
       ------------------------------------------------------------------ */
    uint64_t found = 0;
    seq_t prev = 2;
    seq_t curr = 3;

    const size_t segOdds = 1 << 20; /* 1 048 576 odd numbers per block */
    uint64_t low = 5;               /* first odd to sieve */

    while (1) {
        uint64_t high = low + 2 * segOdds; /* exclusive upper bound */

        char *block = (char *)calloc(segOdds, 1);
        if (!block) return 0; /* memory failure */

        /* mark composite odds in the block */
        for (size_t i = 0; i < smallPrimeCount &&
             (uint64_t)smallPrimes[i] * (uint64_t)smallPrimes[i] <= high; ++i) {
            uint32_t p = smallPrimes[i];
            if (p == 2) continue; /* skip 2, we only sieve odds */

            uint64_t p64 = (uint64_t)p;
            uint64_t start = (low + p64 - 1) / p64 * p64;
            if (start < p64 * p64) start = p64 * p64;
            if ((start & 1ULL) == 0) start += p64;   /* start on odd */

            for (uint64_t j = start; j < high; j += p64 * 2)
                block[(j - low) / 2] = 1;            /* composite */
        }

        /* scan the block for primes */
        for (size_t i = 0; i < segOdds; ++i) {
            if (!block[i]) {
                seq_t nextPrime = low + (seq_t)(2 * i);

                /* check if 'curr' is balanced of order one and ends in 9 */
                seq_t gap1 = curr - prev;
                seq_t gap2 = nextPrime - curr;
                if (gap1 == gap2 && (curr % 10) == 9) {
                    ++found;
                    if (found == (uint64_t)n) {
                        free(block);
                        return curr;
                    }
                }

                /* slide the window */
                prev = curr;
                curr = nextPrime;
            }
        }

        free(block);
        low = high;
    }
}
