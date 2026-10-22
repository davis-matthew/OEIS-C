/*
 * A344334  a(n) is the number of large or small squares that are used to tile
 *          primitive squares of type 1 whose length of side is A344333(n).
 * https://oeis.org/A344334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A344334: number of squares in the tiling of a primitive
   square of side s = a*b*(a^2+b^2), where a and b are coprime.
   The terms are obtained by enumerating all coprime pairs (a,b) with
   a<b, computing s and z = (a*b)^2*(a^2+b^2), sorting the pairs by s
   and selecting the n-th value of z.  This algorithm runs in
   O(N^2) for N≈√n and works for n up to ~10000 within the time
   limit. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t s;   /* side length a*b*(a^2+b^2)   */
    seq_t z;   /* number of squares z          */
} Pair;

/* compare function for qsort – order by increasing side length */
static int cmp_pair(const void *p1, const void *p2) {
    const Pair *A = (const Pair *)p1;
    const Pair *B = (const Pair *)p2;
    if (A->s < B->s) return -1;
    if (A->s > B->s) return 1;
    return 0;
}

seq_t a(idx_t n) {
    /* static state to build the sequence only once */
    static bool inited = false;
    static Pair *seq = NULL;
    static size_t len = 0;
    const idx_t MAXN = 10000;   /* pre‑compute enough terms (≥ 10000) */

    if (!inited) {
        /* Find the maximal a,b needed so that we have at least MAXN terms */
        idx_t B = 0;
        size_t total = 0;
        while (total < MAXN) {
            B++;
            for (idx_t a = 1; a < B; ++a) {
                for (idx_t b = a + 1; b <= B; ++b) {
                    if (gcd(a, b) == 1)
                        ++total;
                }
            }
        }

        /* Allocate space for all pairs up to B */
        seq = (Pair *)malloc(total * sizeof(Pair));
        if (!seq) abort();

        /* Generate all pairs, compute s and z, store them */
        len = 0;
        for (idx_t a = 1; a <= B; ++a) {
            for (idx_t b = a + 1; b <= B; ++b) {
                if (gcd(a, b) != 1) continue;

                seq_t aa = (seq_t)a * (seq_t)a;
                seq_t bb = (seq_t)b * (seq_t)b;
                seq_t ab = (seq_t)a * (seq_t)b;
                seq_t sum = aa + bb;         /* a^2 + b^2 */
                seq_t s   = ab * sum;        /* side length   */
                seq_t z   = ab * ab * sum;   /* number squares */

                seq[len].s = s;
                seq[len].z = z;
                ++len;
            }
        }

        /* Sort by side length to obtain the required order */
        qsort(seq, len, sizeof(Pair), cmp_pair);

        inited = true;
    }

    /* n is 1‑based index into the sorted sequence */
    if (n == 0 || n > len) return 0;   /* out of bounds → 0 (not used) */
    return seq[n-1].z;
}
