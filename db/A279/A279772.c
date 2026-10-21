/*
 * A279772  Numbers n such that the sum of digits of 2n equals 4.
 * https://oeis.org/A279772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A279772: Numbers n such that the sum of digits of 2*n equals 4. */
/* The implementation generates all even numbers m with digit sum 4,
   and returns n = m / 2 for the n‑th smallest such n (1‑based index). */

#include <stdlib.h>

 /* Static storage for generated terms */
static seq_t *cache           = NULL;   /* array of computed n values   */
static size_t cache_size      = 0;      /* number of terms stored       */
static size_t cache_capacity  = 0;      /* allocated size               */
static size_t last_len        = 0;      /* last processed digit length  */

/* Append a new term to the cache, enlarging it if necessary */
static void push_cache(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = (cache_capacity == 0) ? 256 : cache_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = new_cap;
    }
    cache[cache_size++] = val;
}

/* Recursive generation of all numbers of a fixed length 'len' with
   digit sum 4.  'pos'   – current digit position (0 = most significant),
   'rem'   – remaining sum to distribute,
   'cur'   – value constructed so far (digits before 'pos'). */
static void gen_len(size_t len, size_t pos, unsigned int rem, seq_t cur)
{
    if (pos == len) {
        if (rem == 0) {                     /* valid digit sum */
            if ((cur & (seq_t)1) == 0) {    /* even m only                     */
                push_cache(cur / (seq_t)2);  /* n = m/2                           */
            }
        }
        return;
    }

    unsigned int maxd = (rem < 9) ? rem : 9;
    size_t start = (pos == 0 && len > 1) ? 1 : 0;           /* no leading zero for multi‑digit */

    for (size_t d = start; d <= maxd; ++d) {
        seq_t new_cur = cur * (seq_t)10 + (seq_t)d;          /* append digit d              */
        gen_len(len, pos + 1, rem - (unsigned)d, new_cur);
    }
}

/* Generate terms until at least 'target' terms are stored in the cache. */
static void generate_until(size_t target)
{
    while (cache_size < target) {
        ++last_len;                                   /* increase length to process */
        gen_len(last_len, 0, 4, (seq_t)0);             /* start building numbers      */
    }
}

/* Public sequence function: a(n) returns the n‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                       /* undefined for n < 1           */

    /* Ensure enough terms have been generated */
    generate_until((size_t)n);

    return cache[(size_t)(n - 1)];
}
