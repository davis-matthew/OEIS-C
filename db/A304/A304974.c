/*
 * A304974  Number of achiral color patterns (set partitions) for a row or
 *          loop of length n using exactly 4 colors (sets).
 * https://oeis.org/A304974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A304974):
   a(n) = [n≡0 (mod 2)] * (S2(n/2+2,4) – S2(n/2+1,4) – 2·S2(n/2,4))
          + [n≡1 (mod 2)] * (2·S2((n+3)/2,4) – 4·S2((n+1)/2,4))
   where S2(m,k) is the Stirling subset number (a008277).  The implementation
   keeps a small cache of S2(n,4) for 0 ≤ n ≤ max_n, where max_n is the
   largest value needed for the current call.  The cache grows lazily on
   demand, so repeated calls are efficient.  Only the standard library
   headers <stdlib.h> and <stdbool.h> are used, together with
   "../utils/mul_small.h" which provides the helper
   `seq_t mul_small(seq_t, unsigned int)` for multiplying a `seq_t`
   value by a small unsigned int.  All arithmetic uses the seq_t type;
   indices use idx_t (unsigned 64‑bit). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t, unsigned int) */

/* Flattened 2‑D cache: 5 columns for k=0…4.
   index = n * 5 + k.  All entries for n > 0 and k < n are filled
   using the recurrence S2(n,k) = S2(n‑1,k‑1) + k·S2(n‑1,k). */
static seq_t *S2_cache = NULL;      /* points to an array of (max_n+1)*5 */
static idx_t  S2_cache_max_n = 0;   /* current maximum n that the cache
                                       contains (inclusive) */

/* Make sure the cache contains rows up to new_max_n.  The function grows
   the cached array if needed and computes the new rows using the
   recurrence. */
static void init_S2_cache(idx_t new_max_n)
{
    if (new_max_n <= S2_cache_max_n)
        return;  /* already sufficient */

    size_t old_len = (size_t)(S2_cache_max_n + 1) * 5;
    size_t new_len = (size_t)(new_max_n + 1) * 5;

    seq_t *new_cache = (seq_t *)realloc(S2_cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();                 /* unrecoverable allocation failure */

    /* If the cache was empty before, initialise the n = 0 row. */
    if (S2_cache_max_n == 0 && S2_cache == NULL) {
        new_cache[0 * 5 + 0] = 1;            /* S2(0,0) = 1 */
        for (idx_t k = 1; k <= 4; ++k)
            new_cache[0 * 5 + k] = 0;        /* S2(0,k>0) = 0 */
    }

    /* Compute the new rows using the recurrence. */
    for (idx_t n = S2_cache_max_n + 1; n <= new_max_n; ++n) {
        size_t base = (size_t)n * 5;
        new_cache[base + 0] = 0;            /* S2(n,0)=0 for n>0 */
        for (idx_t k = 1; k <= 4; ++k) {
            seq_t left  = new_cache[(size_t)(n - 1) * 5 + (k - 1)];
            seq_t right = new_cache[(size_t)(n - 1) * 5 + k];
            new_cache[base + k] = left + mul_small(right, (unsigned int)k);
        }
    }

    S2_cache = new_cache;
    S2_cache_max_n = new_max_n;
}

/* Return S2(n,k) for 0 ≤ k ≤ 4.  The function expands the cache if
   necessary.  For k>n the cached value is 0 (by construction). */
static seq_t get_S2(idx_t n, idx_t k)
{
    if (n > S2_cache_max_n)
        init_S2_cache(n);
    return S2_cache[(size_t)n * 5 + k];
}

/* Main entry – compute a(n) for the OEIS sequence A304974. */
seq_t a(idx_t n)
{
    /* Odd indices: 2*S2((n+3)/2,4) – 4*S2((n+1)/2,4)
       Even indices: S2(n/2+2,4) – S2(n/2+1,4) – 2*S2(n/2,4)   */
    if ((n & 1) == 0) {          /* even n */
        idx_t m  = n / 2;
        idx_t n1 = m + 2;        /* n/2 + 2 */
        idx_t n2 = m + 1;        /* n/2 + 1 */
        idx_t n3 = m;            /* n/2         */
        init_S2_cache(n1);       /* ensure all needed rows exist */
        seq_t s1 = get_S2(n1, 4);
        seq_t s2 = get_S2(n2, 4);
        seq_t s3 = get_S2(n3, 4);
        seq_t tmp  = s1 - s2;                  /* s1 – s2 */
        seq_t res  = tmp - mul_small(s3, 2);    /* – 2*s3 */
        return res;
    } else {                     /* odd n */
        idx_t n1 = (n + 3) / 2;  /* (n+3)/2   */
        idx_t n2 = (n + 1) / 2;  /* (n+1)/2   */
        init_S2_cache(n1);       /* ensure rows up to n1 */
        seq_t s1 = get_S2(n1, 4);
        seq_t s2 = get_S2(n2, 4);
        seq_t res = mul_small(s1, 2) - mul_small(s2, 4);
        return res;
    }
}
