/*
 * A293263  Number of fixed polyominoes without holes that have a width of n
 *          and height of 3.
 * https://oeis.org/A293263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 34*a(n-2) + 49*a(n-3) - 29*a(n-4) * +
 *            2*a(n-5) + 10*a(n-6) - 5*a(n-7) - 2*a(n-8) * for n > 9, * with
 *            a(1) = 1, a(2) = 15, a(3) = 106, a(4) = 582, * a(5) = 2952, a(6)
 *            = 14488, a(7) = 69982, * a(8) = 335356, a(9) = 1600624. Missing
 *            values for small types * are produced modulo the...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, realloc */

static const seq_t init_terms[10] = {
    (seq_t)0,          /* unused index 0 */
    (seq_t)1,          /* a(1)  */
    (seq_t)15,         /* a(2)  */
    (seq_t)106,        /* a(3)  */
    (seq_t)582,        /* a(4)  */
    (seq_t)2952,       /* a(5)  */
    (seq_t)14488,      /* a(6)  */
    (seq_t)69982,      /* a(7)  */
    (seq_t)335356,     /* a(8)  */
    (seq_t)1600624     /* a(9)  */
};

static seq_t *cache = NULL;          /* memoised values, index 0 unused */
static size_t cache_capacity = 0;    /* allocated array size (>=1) */
static idx_t  computed_max  = 0;     /* largest n whose a(n) is known */

/* Compute a(n) for any n >= 1.  For n==0 returns 0 (unspecified). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Grow cache to hold indices up to n.  Use power‑of‑two growth. */
    if ((size_t)n >= cache_capacity) {
        size_t new_cap = (cache_capacity == 0) ? 16 : cache_capacity * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *temp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!temp) {            /* allocation failure – abort */
            abort();
        }
        cache = temp;
        cache_capacity = new_cap;
    }

    /* Initialise memoisation if needed. */
    if (computed_max == 0 && cache_capacity > 0) {
        cache[0] = (seq_t)0;          /* unused slot */
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init_terms[i];
        } else {
            seq_t val = 10 * cache[i - 1];
            val -= 34 * cache[i - 2];
            val += 49 * cache[i - 3];
            val -= 29 * cache[i - 4];
            val += 2 *  cache[i - 5];
            val += 10 * cache[i - 6];
            val -= 5 *  cache[i - 7];
            val -= 2 *  cache[i - 8];
            cache[i] = val;
        }
    }

    computed_max = n;
    return cache[n];
}
