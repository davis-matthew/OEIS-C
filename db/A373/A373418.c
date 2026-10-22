/*
 * A373418  Triangle read by rows: T(n,k) is the number of permutations in
 *          symmetric group S_n with (n-k) fixed points and an odd number of
 *          non-fixed point cycles. Equivalent to the number of cycles of n
 *          items with cycle type defined by non-unity partitions of k <= n
 *          that contain an odd number of parts.
 * https://oeis.org/A373418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = ⌊(√(8n+1)-1)/2⌋ and c = n - r(r+1)/2
 *            T(r,c) = binomial(r,c) * ( D_c + c - 1 ) / 2 with D_0 = 1, D_1 =
 *            0 and D_k = (k-1)*(D_{k-1}+D_{k-2}) for k≥2
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Cache of derangement numbers D_k.  D_cache[i] holds D_i. */
static seq_t *D_cache = NULL;
static idx_t   D_cache_size = 0;   /* number of entries presently allocated */

/* Ensure that D_cache contains values D_0 … D_k.  The array is
   reallocated (if necessary) and new entries are computed on demand. */
static void ensure_D(idx_t k)
{
    if (k + 1 <= D_cache_size)
        return;

    idx_t new_size = k + 1;
    seq_t *new_cache = (seq_t *)realloc(D_cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure: terminate the program – the sequence
           cannot be continued. */
        abort();
    }
    D_cache = new_cache;

    /* Compute new entries, starting from the previous size. */
    for (idx_t i = D_cache_size; i < new_size; ++i) {
        if (i == 0) {
            D_cache[i] = (seq_t)1;
        } else if (i == 1) {
            D_cache[i] = (seq_t)0;
        } else {
            seq_t sum = D_cache[i-1] + D_cache[i-2];
            D_cache[i] = ((seq_t)(i-1)) * sum;
        }
    }
    D_cache_size = new_size;
}

/* Compute the triangular entry T(r,c) for 0≤c≤r. */
static seq_t T_val(idx_t r, idx_t c)
{
    if (c == 0 || c == 1)
        return (seq_t)0;              /* T(r,0) and T(r,1) are zero */

    ensure_D(c);

    seq_t Dk       = D_cache[c];
    seq_t numerator= Dk + (seq_t)(c - 1);     /* D_c + c - 1, always even */
    seq_t half     = numerator / (seq_t)2;    /* integer division */

    seq_t comb     = binomial(r, c);          /* C(r,c) */
    return comb * half;
}

/* Flat triangular sequence requested by the harness. */
seq_t a(idx_t n)
{
    /* Determine row r and column c that correspond to linear index n. */
    idx_t r = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t c = n - start_of_row;

    if (c > r)            /* malformed index – return 0 safety */
        return (seq_t)0;

    return T_val(r, c);
}
