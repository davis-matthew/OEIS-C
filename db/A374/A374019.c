/*
 * A374019  Expansion of Product_{k>=1} 1 / (1 - x^(4*k-1))^2.
 * https://oeis.org/A374019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   OEIS A374019:  a(n) = Sum_{k=0..n} A035462(k) * A035462(n-k)
   where A035462(k) is the number of partitions of k into parts congruent
   to 1 (mod 4).  The generating function is
       Product_{m>=1} 1/(1 - x^{4*m-1})^2
   and the convolution description above is used below.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cache of the partition numbers b(n) and the convolution a(n). */
static seq_t *b = NULL;          /* b[i] = number of partitions of i into 1 mod 4 parts */
static seq_t *a_cache = NULL;    /* a_cache[i] = desired sequence value */
static idx_t max_n = 0;          /* highest n for which arrays are defined */

/* Recompute both arrays up to (and including) n.  The function
   is called only when a larger n is requested, so the total work
   is O(n^2) where n is the maximal index encountered. */
static void recompute_up_to(idx_t n)
{
    /* Allocate space for the new arrays.  realloc keeps existing data
       but we will recompute everything anyway. */
    size_t sz = (size_t)(n + 1);
    seq_t *new_b = (seq_t *)realloc(b, sz * sizeof(seq_t));
    seq_t *new_a = (seq_t *)realloc(a_cache, sz * sizeof(seq_t));
    if (!new_b || !new_a) {
        abort();                     /* out of memory – terminate safely */
    }
    b = new_b;
    a_cache = new_a;

    /* Initialise all entries to zero. */
    for (size_t i = 0; i < sz; ++i) {
        b[i] = 0;
        a_cache[i] = 0;
    }

    /* b[0] = 1 by definition of the empty partition. */
    b[0] = 1;

    /* Compute b(i) for all i ≤ n using the classic partition DP.
       Allowed parts are 4k-1 (k ≥ 1). */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t part = 4 * k - 1;
        if (part > n) break;
        for (size_t i = (size_t)part; i <= n; ++i) {
            b[i] += b[i - part];
        }
    }

    /* Compute a(i) = Σ_{j=0..i} b[j] * b[i-j] for all i ≤ n. */
    a_cache[0] = 1;          /* b[0]*b[0] */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j <= i; ++j) {
            sum += b[j] * b[i - j];
        }
        a_cache[i] = sum;
    }

    max_n = n;
}

/* External linkage as requested. */
seq_t a(idx_t n)
{
    /* If this is the first call or the cached arrays are too small,
       rebuild them up to the desired index. */
    if ((b == NULL) || (n > max_n)) {
        recompute_up_to(n);
    }
    return a_cache[n];
}
