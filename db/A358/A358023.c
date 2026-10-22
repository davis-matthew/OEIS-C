/*
 * A358023  Number of partitions of n into at most 2 distinct squarefree
 *          parts.
 * https://oeis.org/A358023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [is_squarefree(n)] + * Σ_{s=1}^{⌊n/2⌋} [is_squarefree(s)
 *            && is_squarefree(n-s)]
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the function required by the build system */
seq_t a(idx_t n);

/* Static data for caching square‑free information */
static char *sq_free = NULL;      /* 1 = square‑free, 0 = not square‑free */
static size_t sq_cap  = 0;        /* current capacity of sq_free array */

/* Ensure that sq_free[0..n] is defined and correct */
static void ensure_squarefree(idx_t n) {
    size_t required = (size_t)n + 1;
    if (required <= sq_cap) return;         /* already computed */

    /* Allocate new array with all entries initially true (square‑free) */
    char *new_arr = (char*)malloc(required);
    if (!new_arr) return;                   /* allocation failed – unlikely */

    for (size_t i = 0; i < required; ++i) new_arr[i] = 1;
    new_arr[0] = 0;                          /* 0 is not a positive part */

    /* Use the naive sieve: numbers divisible by a square are not square‑free */
    for (idx_t i = 2; i * i <= n; ++i) {
        idx_t sq = i * i;
        for (idx_t m = sq; m <= n; m += sq) {
            new_arr[m] = 0;
        }
    }

    /* Replace old array */
    if (sq_free) free(sq_free);
    sq_free = new_arr;
    sq_cap  = required;
}

seq_t a(idx_t n) {
    /* The empty partition is defined to be the only partition of 0 */
    if (n == 0) return 1;

    ensure_squarefree(n);

    seq_t count = 0;

    /* Partitions with a single distinct part */
    if (sq_free[n]) ++count;

    /* Partitions with two distinct parts (unordered) */
    for (idx_t s = 1; s <= n / 2; ++s) {
        if (!sq_free[s]) continue;
        idx_t t = n - s;
        if (t <= s) continue;           /* avoid double counting and ensure distinctness */
        if (sq_free[t]) ++count;
    }

    return count;
}
