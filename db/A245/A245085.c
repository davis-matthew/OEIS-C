/*
 * A245085  Maximal t in [2, prime(n)-1] such that all the smallest positive
 *          residues of 2!,3!,...,t! modulo prime(n) are distinct.
 * https://oeis.org/A245085
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 6746, u32 n <= 9805, u64 n <= 9805,
 *            u128 n <= 9805, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A245085: for the n-th prime p, find the greatest
 * t in [2, p‑1] such that the residues of 2!, 3!, … , t! (mod p)
 * are pairwise distinct.  The search stops at the first duplicate
 * residue; if none occurs up to p‑1 then t = p‑1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;

    /* Get the n‑th prime (1‑based).  The helper returns a 32‑bit
       value sufficient for all tests up to several thousand indices. */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;          /* prime as seq_t for arithmetic */

    /* Allocate a bitmap of length p to remember seen residues.  */
    size_t sz = (size_t)p32;
    uint8_t *seen = (uint8_t*)calloc(sz, 1);
    if (!seen) return (seq_t)0;    /* memory failure – unlikely in tests */

    seq_t fact = 1;                /* current factorial modulo p */
    seq_t t = 0;                   /* answer */

    for (idx_t k = 2; k < (idx_t)p; ++k) {
        fact = (fact * (seq_t)k) % p;
        idx_t r = (idx_t)fact;    /* residue fits in idx_t because r < p */
        if (seen[r]) {            /* duplicate residue found */
            t = (seq_t)(k - 1);
            break;
        }
        seen[r] = 1;
    }
    if (t == 0) t = (seq_t)(p - 1);   /* no duplicate seen */

    free(seen);
    return t;
}
