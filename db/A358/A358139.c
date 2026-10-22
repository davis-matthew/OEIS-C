/*
 * A358139  Numbers k > 0 sorted by k/A000120(k) in increasing order. A000120
 *          is the binary weight of k. If k/A000120(k) yields equal values,
 *          the smaller k will appear first.
 * https://oeis.org/A358139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation: a(n) is the n-th element of the permutation of positive
   integers sorted by k / popcount(k) (popcount = A000120(k)).  To obtain a
  (k) for any n we generate all numbers up to 2*n, sort them by that
   ratio, and return the (n‑1)-th element of the sorted list.
*/
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Comparator for sorting indices by the ratio k / popcount(k). */
static int cmp_ratio(const void *a, const void *b)
{
    idx_t k1 = *(const idx_t *)a;
    idx_t k2 = *(const idx_t *)b;

    /* popcount for 64-bit values */
    uint64_t pop1 = __builtin_popcountll((uint64_t)k1);
    uint64_t pop2 = __builtin_popcountll((uint64_t)k2);

    /* Use 128‑bit to avoid overflow in cross multiplication. */
    unsigned __int128 left  = (unsigned __int128)k1 * pop2;
    unsigned __int128 right = (unsigned __int128)k2 * pop1;

    if (left < right)  return -1;
    if (left > right)  return  1;
    /* tie: smaller k first */
    if (k1 < k2) return -1;
    if (k1 > k2) return  1;
    return 0;
}

/* Static state to avoid re‑computing beyond the needed range. */
static idx_t *sorted_k = NULL;   /* array of numbers sorted by ratio */
static size_t sorted_len = 0;    /* current size of sorted_k */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* We need at least 2*n numbers to guarantee the nth element is
       within the sorted array.  Re‑compute if the current range
       isn't large enough. */
    if ((size_t)(2 * n) > sorted_len) {
        /* New length to compute */
        size_t L = (size_t)2 * n;
        idx_t *buf = (idx_t *)malloc(L * sizeof(idx_t));
        assert(buf != NULL);

        for (idx_t i = 1; i <= (idx_t)L; ++i) {
            buf[i - 1] = i;
        }
        qsort(buf, L, sizeof(idx_t), cmp_ratio);

        /* Replace old buffer if any */
        free(sorted_k);
        sorted_k = buf;
        sorted_len = L;
    }

    /* Return the (n-1)-th element in the 1‑indexed sequence. */
    return (seq_t)sorted_k[n - 1];
}
