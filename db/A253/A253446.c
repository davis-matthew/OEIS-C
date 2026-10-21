/*
 * A253446  Indices of centered heptagonal numbers (A069099) which are also
 *          centered octagonal numbers (A016754).
 * https://oeis.org/A253446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 31*a(n-2) + a(n-3), a(1)=1, a(2)=16, a(3)=465
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of computed terms, indices 0..cache_size */
    static idx_t cache_size = 0;      /* maximum index currently allocated  */
    static idx_t computed = 0;        /* highest index whose value is already in cache */

    if (n == 0) return 0;               /* undefined for offset 1 */

    /* Allocate more space if necessary */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;             /* allocation failure – return zero */
        cache = tmp;
        cache_size = n;
    }

    /* Ensure the first three terms are present (only write if index exists) */
    if (computed < 1 && cache_size >= 1) cache[1] = (seq_t)1;
    if (computed < 2 && cache_size >= 2) cache[2] = (seq_t)16;
    if (computed < 3 && cache_size >= 3) cache[3] = (seq_t)465;

    /* Compute terms up to n */
    idx_t start = computed + 1;
    if (start < 4) start = 4;           /* recurrence only applies for n >= 4 */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = (seq_t)31 * cache[i-1] - (seq_t)31 * cache[i-2] + cache[i-3];
    }

    if (n > computed) computed = n;
    return cache[n];
}
