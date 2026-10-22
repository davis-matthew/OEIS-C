/*
 * A325269  Number of integer partitions of n with 2 distinct parts or at
 *          least 3 parts.
 * https://oeis.org/A325269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) - (n%2==0?2:1) for n>0, a(0)=0
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = p(n) – 1 if n is odd,  p(n) – 2 if n is even,
   where p(n) is the partition number A000041(n).  The partition numbers
   are computed once and cached.  The implementation uses only integer
   arithmetic and standard C headers. */

#include <stdlib.h>

static seq_t *partition_cache = NULL;   /* cache of partition numbers   */
static size_t cache_size = 0;           /* number of elements in cache  */

static void ensure_partitions(idx_t nmax)
{
    if ((size_t)nmax + 1 <= cache_size)
        return;                         /* already computed up to nmax */

    size_t newsize = (size_t)nmax + 1;
    seq_t *newp = (seq_t *)calloc(newsize, sizeof(seq_t));
    if (!newp)
        return;                         /* allocation failure – unlikely */

    newp[0] = 1;                         /* p(0) = 1 */

    /* Classic DP: for each part size i add it to larger sums */
    for (idx_t i = 1; i <= nmax; ++i) {
        for (idx_t j = i; j <= nmax; ++j) {
            newp[(size_t)j] += newp[(size_t)(j - i)];
        }
    }

    free(partition_cache);
    partition_cache = newp;
    cache_size = newsize;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset 0 value */

    ensure_partitions(n);
    seq_t part = partition_cache[(size_t)n];

    if (n & 1)                            /* odd n: subtract 1   */
        return part - 1;
    else                                 /* even n: subtract 2  */
        return part - 2;
}
