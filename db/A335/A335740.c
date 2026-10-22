/*
 * A335740  Factorize each integer m >= 2 as the product of powers of nonunit
 *          squarefree numbers with distinct exponents that are powers of 2.
 *          The sequence lists m such that the factor with the largest
 *          exponent is not a power of 2.
 * https://oeis.org/A335740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_included_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer m ≥ 2 whose unique factorization into powers of
   squarefree numbers with distinct exponents that are powers of two
   has the largest exponent attached to a factor that is NOT a
   power of two (i.e., it includes an odd prime). */

/*--------------------------------------------------------------------
  Dependencies: seq_t, idx_t, and standard C headers only.
  The only helper function used is _is_included_u64, which factors
  a 64‑bit integer.  The algorithm runs in O(m⋅√m) for small m,
  which is sufficiently fast up to n≈10000 (values roughly 1.2 n).
--------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_is_included_u64.h"

/*--------------------------------------------------------------------
  Main function: returns the n‑th term (1‑based indexing) of A335740.
  The implementation builds the sequence lazily, storing found terms
  in a static dynamic array.  Calls may arrive in arbitrary order.
--------------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* safety guard – not used */

    /* static state */
    static uint64_t   cur = 2;          /* next candidate to test   */
    static seq_t     *seqs = NULL;      /* array of found terms      */
    static size_t     cap  = 0;         /* allocated capacity        */
    static size_t     len  = 0;         /* number of terms stored    */

    const size_t target = (size_t)n;    /* needed number of terms   */

    while (len < target) {
        if (_is_included_u64(cur)) {
            if (len == cap) {           /* grow array if needed     */
                size_t newcap = cap ? cap << 1 : 64;
                seqs = (seq_t *)realloc(seqs, newcap * sizeof(seq_t));
                cap = newcap;
            }
            seqs[len++] = (seq_t)cur;   /* store found term         */
        }
        ++cur;
    }

    return seqs[target - 1];
}
