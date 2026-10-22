/*
 * A383396  Primes p such that p + 6, p + 10, p + 12, p + 16 and p + 22 are
 *          also primes.
 * https://oeis.org/A383396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383396 – primes p such that p+6, p+10, p+12, p+16, p+22 are also primes.
   The function performs an incremental search, memoising all found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    static idx_t   count = 0;          /* terms generated so far        */
    static seq_t *vals  = NULL;        /* dynamic array of terms        */
    static size_t  cap   = 0;          /* current capacity of vals      */
    static seq_t  cand   = 7;          /* next odd candidate to test    */

    if (n == 0) return 0;              /* undefined for 0, but safe */

    /* Initial allocation of the values array. */
    if (!vals) {
        cap = 16;
        vals = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        /* Skip non‑prime candidates. */
        while (!isPrimeGeneric(cand)) {
            cand += (seq_t)2;          /* stay odd */
        }

        /* Test sextuple pattern. */
        if (isPrimeGeneric(cand + (seq_t)6)  &&
            isPrimeGeneric(cand + (seq_t)10) &&
            isPrimeGeneric(cand + (seq_t)12) &&
            isPrimeGeneric(cand + (seq_t)16) &&
            isPrimeGeneric(cand + (seq_t)22)) {

            /* Store the found prime. */
            if (count == cap) {
                cap <<= 1;
                vals = (seq_t*)realloc(vals, cap * sizeof(seq_t));
            }
            vals[count++] = cand;
        }

        cand += (seq_t)2;              /* move to next odd number     */
    }

    return vals[n - 1];
}
