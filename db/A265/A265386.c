/*
 * A265386  Sequence defined by a(1)=a(2)=1 and a(n) = gray(gray(a(n-1)) +
 *          gray(a(n-2))), with gray(m) = A003188(m).
 * https://oeis.org/A265386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = gray( gray(a(n-1)) + gray(a(n-2)) ),
 *            where gray(m) = m XOR (m >> 1). (binary-reflected Gray code)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   gray_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */
#include "../utils/gray_value.h"

static seq_t *seq_cache = NULL;   /* 1‑based array: seq_cache[1] = a(1) etc. */
static idx_t  seq_cache_size = 0; /* current maximum index stored (≥1) */

/* Ensure cache contains all terms up to index n, computing missing terms. */
static void seq_ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {                     /* first call */
        seq_cache_size = n >= 2 ? n : 2;          /* allocate at least 2 slots */
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * (seq_cache_size + 1));
        if (!seq_cache) exit(EXIT_FAILURE);      /* allocation failed */

        /* initialise seed values */
        seq_cache[1] = 1;
        seq_cache[2] = 1;

        /* compute any remaining terms up to seq_cache_size */
        for (idx_t k = 3; k <= seq_cache_size; ++k) {
            seq_t g1 = gray_value(seq_cache[k - 1]);
            seq_t g2 = gray_value(seq_cache[k - 2]);
            seq_cache[k] = gray_value(g1 + g2);
        }
    }
    else if (n > seq_cache_size) {               /* enlarge cache */
        idx_t old_size = seq_cache_size;
        seq_cache_size = n;
        seq_cache = (seq_t *)realloc(seq_cache, sizeof(seq_t) * (seq_cache_size + 1));
        if (!seq_cache) exit(EXIT_FAILURE);      /* allocation failed */

        for (idx_t k = old_size + 1; k <= seq_cache_size; ++k) {
            seq_t g1 = gray_value(seq_cache[k - 1]);
            seq_t g2 = gray_value(seq_cache[k - 2]);
            seq_cache[k] = gray_value(g1 + g2);
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* offset starts at 1 */
    seq_ensure_cache(n);
    return seq_cache[n];
}
