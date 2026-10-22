/*
 * A353106  Base-6 representation of A007908(n).
 * https://oeis.org/A353106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base‑6 representation of the decimal number obtained by *
 *            concatenating the integers 1,2,…,n. More precisely, * a(n) =
 *            base‐6(A007908(n)), where A007908(n) = concat(1,…,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

static seq_t *cache = NULL;           /* cache of already computed a(n) */
static size_t cache_cap = 0;          /* allocated capacity of cache array */

static seq_t base6_of_concat(idx_t n)
{
    seq_t concat = 0;

    /* build the decimal concatenation 1…n */
    for (idx_t k = 1; k <= n; ++k) {
        int dk = digits_of(k);                 /* number of decimal digits of k */
        seq_t p   = pow10((idx_t)dk);           /* 10^dk as seq_t */
        concat = concat * p + (seq_t)k;        /* append k */
    }

    /* convert the big integer `concat` to base‑6 */
    seq_t tmp = concat;
    size_t cap = 256;            /* initial capacity for reversed base‑6 digits */
    size_t len = 0;
    uint8_t *rev = (uint8_t *)malloc(cap);
    if (!rev) return (seq_t)0;   /* out of memory – return 0 */

    while (tmp > (seq_t)0) {
        uint8_t d = (uint8_t)(tmp % (seq_t)6);
        tmp      = tmp / (seq_t)6;
        if (len == cap) {
            cap *= 2;
            uint8_t *tmpptr = (uint8_t *)realloc(rev, cap);
            if (!tmpptr) { free(rev); return (seq_t)0; }
            rev = tmpptr;
        }
        rev[len++] = d;
    }
    if (len == 0) {             /* never happens for n>=1; guard anyway */
        rev[len++] = 0;
    }

    /* reconstruct the decimal integer whose digits are the base‑6 digits */
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * (seq_t)10 + (seq_t)rev[len - 1 - i];
    }
    free(rev);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* offset is 1, undefined below */

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1024;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        /* initialize new entries to 0 (sentinel for "not computed") */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (cache[n] != (seq_t)0) return cache[n];

    /* compute, cache, and return */
    seq_t val = base6_of_concat(n);
    cache[n] = val;
    return val;
}
