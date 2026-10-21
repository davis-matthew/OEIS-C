/*
 * A261929  a(n) is the number of different pairs (p,q) mod n not of the form
 *          (x*y,x+y) mod n for any (x,y).
 * https://oeis.org/A261929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A261929 – number of pairs (p,q) mod n that are NOT of the form
 *  (x*y mod n , x+y mod n) for any pair (x,y) in Z_n×Z_n.
 *
 *  For each n ≥ 1:
 *      Let S = {(x*y mod n , (x+y) mod n) | 0 ≤ x,y < n}
 *      a(n) = n^2 – |S|
 *
 *  The function below explicitly builds the set S using a bit‑set for
 *  the n^2 possible pairs.  Complexity:  O(n^2) operations and
 *  O(n^2/8) bytes of memory.  For n ≤ 10 000 this is < 13 MiB and
 *  completes in well under two seconds on typical hardware.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --- core function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used; offset is 1 */

    size_t N = (size_t)n;                /* work with native unsigned types   */
    size_t total = N * N;                /* number of all pairs                */
    size_t words = (total + 63) / 64;    /* words of 64 bits for bit‑set       */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) return 0;                 /* memory allocation failure          */

    size_t count = 0;                    /* number of distinct reachable pairs */

    /* Build the image S by scanning all (x,y). */
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < N; ++y) {
            uint64_t p = ((uint64_t)x * (uint64_t)y) % N; /* xy mod n   */
            uint64_t q = (x + y) % N;                    /* x + y mod n*/
            size_t idx = (size_t)p * N + q;              /* linear index */

            size_t w = idx >> 6;      /* word index   */
            unsigned b = idx & 63;    /* bit index    */
            uint64_t mask = 1ULL << b;

            if (!(bits[w] & mask)) { /* first time we see this pair */
                bits[w] |= mask;
                ++count;
            }
        }
    }

    free(bits);

    seq_t totalPairs = (seq_t)N * ((seq_t)N);   /* n^2  */
    seq_t result = totalPairs - (seq_t)count;   /* a(n) */

    return result;
}
