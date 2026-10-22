/*
 * A366786  a(n) = A073481(n)*A005117(n).
 * https://oeis.org/A366786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A366786: a(n) = nth squarefree number * least prime factor of that number.
   a(1) = 1; for n>1, let s(n) be the nth squarefree integer, then
   a(n) = s(n) * minprime(s(n)) where minprime(1)=1. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but safe */
    if (n == 1) return (seq_t)1;       /* a(1)=1 by definition */

    /* Upper bound for the nth squarefree number:
       density of squarefree integers is 6/π² ≈ 0.6079, so n*2 is safe for n up to 10⁶. */
    idx_t limit = (idx_t)((double)n * 2.0);
    if (limit < 3) limit = 3;

    /* Allocate arrays */
    uint64_t *lpf = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    uint8_t   *squarefree = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!lpf || !squarefree) return (seq_t)0;  /* allocation failed */

    /* Initialise */
    for (uint64_t i = 0; i <= limit; ++i) {
        squarefree[i] = 1;   /* assume squarefree initially */
        lpf[i] = 0;          /* to be filled by sieve */
    }
    squarefree[0] = 0;  /* 0 is not squarefree by definition */

    /* Sieve for least prime factor */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (lpf[i] == 0) {                /* i is prime */
            lpf[i] = i;
            if (i <= limit / i) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    if (lpf[j] == 0) lpf[j] = i;
                }
            }
        }
    }

    /* Mark numbers that have a square factor */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        uint64_t sq = p * p;
        for (uint64_t j = sq; j <= limit; j += sq) {
            squarefree[j] = 0;
        }
    }

    /* Find the nth squarefree number */
    idx_t count = 0;
    uint64_t s_n = 0;
    for (uint64_t i = 1; i <= limit; ++i) {
        if (squarefree[i]) {
            ++count;
            if (count == n) {
                s_n = i;
                break;
            }
        }
    }

    uint64_t lpf_val = 0;
    if (s_n >= 2) lpf_val = lpf[s_n];
    else lpf_val = 1;  /* by definition for 1 */

    seq_t result = (seq_t)s_n * (seq_t)lpf_val;

    free(lpf);
    free(squarefree);

    return result;
}
