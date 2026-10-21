/*
 * A254960  Zeroless numbers n with digits d_1, d_2, ... d_k such that d_1^3 +
 *          ... + d_k^3 is a cube.
 * https://oeis.org/A254960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 336, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Zeroless numbers n with digits d₁,…,d_k such that
 * d₁³ + … + d_k³ is a perfect cube.
 * Computed by brute‑force enumeration and caching. */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Pre‑computed cubes of 0…9 */
static const seq_t CUBES[10] = {
    (seq_t)0, (seq_t)1, (seq_t)8, (seq_t)27, (seq_t)64,
    (seq_t)125, (seq_t)216, (seq_t)343, (seq_t)512, (seq_t)729
};

/* Static cache for the sequence */
static seq_t *SEQ = NULL;
static size_t SEQ_CAP = 0;
static size_t SEQ_LEN = 0;
static seq_t CUR = (seq_t)1;        /* next number to test */

/* Ensure the cache has room for at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need <= SEQ_CAP) return;
    size_t new_cap = SEQ_CAP ? SEQ_CAP << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    SEQ = seq_realloc(SEQ, SEQ_CAP, new_cap);
    SEQ_CAP = new_cap;
}

/* Test whether a number has no zero digits and that
 * the sum of the cubes of its digits is a perfect cube. */
static bool test_candidate(seq_t n)
{
    seq_t sum = 0;
    seq_t t = n;
    while (t > 0) {
        seq_t d = t % 10;
        if (d == 0) return false;          /* contains a zero → skip */
        sum += CUBES[(int)d];
        t /= 10;
    }
    seq_t c = floor_cube_root(sum);
    return c * c * c == sum;
}

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* low‐index behaviour irrelevant */

    /* Return cached value if already computed */
    if ((size_t)(n - 1) < SEQ_LEN) return SEQ[n - 1];

    /* Generate terms until we reach the desired index */
    while (SEQ_LEN < n) {
        if (test_candidate(CUR)) {
            ensure_capacity(SEQ_LEN + 1);
            SEQ[SEQ_LEN++] = CUR;
        }
        CUR++;
    }
    return SEQ[n - 1];
}
