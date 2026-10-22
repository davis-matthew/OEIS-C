/*
 * A338872  Square array of distinct positive integers T(n, k), n > 0, k > 0,
 *          read by antidiagonals upwards, filled the greedy way such that for
 *          any n > 0 and k > 0, Sum_{ i = 1..n, j = 1..k } T(i, j) is
 *          divisible by n * k.
 * https://oeis.org/A338872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1201, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction of the square array T(n,k) with the property that
   for any n>0 and k>0, the sum of all entries T(i,j) with 1 ≤ i ≤ n,
   1 ≤ j ≤ k is divisible by n*k.  a(n) is the n-th entry read by
   antidiagonals upwards (start with T(1,1), then T(2,1), T(1,2), T(3,1),
   T(2,2), T(1,3), …).  */
#include <stdlib.h>
#include <string.h>

static seq_t      *cache    = NULL;   /* sequence values, cache[0] = a(1) */
static size_t     cache_len = 0;      /* number of values stored          */
static size_t     cache_cap = 0;      /* allocated capacity               */

/* used[i] == 1 iff the integer i has already appeared in the array
   (indices are 1‑based; element 0 is never used).  */
static unsigned char *used     = NULL;
static size_t        used_cap = 0;  /* allocated capacity */

/* prefix sums S(i,j) of the rectangle from (1,1) to (i,j).  The size must
   cover all cells that may appear while generating up to 10 000 terms
   (≈ √(2⋅10 000) ≈ 150).  A buffer of 1000 is ample and inexpensive.  */
#define MAX_DIM 1000
static seq_t ps[MAX_DIM+1][MAX_DIM+1];

/* current antidiagonal to process (sum = i + j) and the position within it.
   For sum s, positions run from 1 to s-1 with i = s - pos, j = pos.      */
static idx_t next_s   = 2;   /* first sum (i+j) to process                   */
static idx_t next_pos = 1;   /* first position within current diagonal      */

/* ensure that the used array can hold an entry with index ci            */
static void ensure_used(size_t ci)
{
    if (ci >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : (1U << 20);
        while (new_cap <= ci) new_cap <<= 1;
        used = (unsigned char*)realloc(used, new_cap);
        memset(used + used_cap, 0, new_cap - used_cap);
        used_cap = new_cap;
    }
}

/* initialize the dynamic arrays if this is the first call                */
static void init(void)
{
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }
    if (!used) {
        used_cap = 1U << 20;  /* 1 MiB of bools                            */
        used = (unsigned char*)calloc(used_cap, 1);
    }
}

/* append a value to the cache, expanding it if necessary                 */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap << 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

seq_t a(idx_t n)
{
    init();

    /* generate terms until we have at least n of them                 */
    while (cache_len < n) {
        idx_t i = next_s - next_pos;   /* row index (≥1)                  */
        idx_t j = next_pos;            /* column index (≥1)                */

        /* current sum of all entries strictly before (i,j)            */
        seq_t top  = (i > 1) ? ps[i-1][j] : 0;
        seq_t left = (j > 1) ? ps[i][j-1] : 0;
        seq_t diag = (i > 1 && j > 1) ? ps[i-1][j-1] : 0;
        seq_t P = top + left - diag;

        seq_t M   = (seq_t)i * (seq_t)j;
        seq_t rem = P % M;
        seq_t r   = (M - rem) % M;
        seq_t start = (r == 0) ? M : r;   /* smallest positive ≡ r (mod M) */

        /* find the smallest unused positive integer ≡ start (mod M)   */
        seq_t cand = start;
        while (1) {
            size_t ci = (size_t)cand;   /* index into used[]            */
            ensure_used(ci);
            if (!used[ci]) {
                used[ci] = 1;          /* mark as used                 */

                /* store new prefix sum                     */
                ps[i][j] = P + cand;

                /* append to sequence                      */
                cache_append(cand);

                /* advance to the next coordinate           */
                next_pos++;
                if (next_pos > next_s - 1) {
                    next_s++;
                    next_pos = 1;
                }
                break;
            }
            cand += M;
        }
    }

    return cache[n-1];
}
