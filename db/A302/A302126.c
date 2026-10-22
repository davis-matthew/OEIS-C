/*
 * A302126  Interleaved Fibonacci and Lucas numbers.
 * https://oeis.org/A302126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(n)=a(n-2)+a(n-4) for n≥4. Equivalent:
 *            a(2k)=Fib(k), a(2k+1)=Lucas(k)
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 94, u64 n <= 186,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Sequences cached across calls */
static seq_t *fib_cache = NULL;
static size_t fib_len = 0;  /* number of computed fibonacci terms */
static size_t fib_cap = 0;  /* capacity of fib_cache array */

static seq_t *lucas_cache = NULL;
static size_t lucas_len = 0;  /* number of computed lucas terms */
static size_t lucas_cap = 0;  /* capacity of lucas_cache array */

/* Ensure that fibonacci cache is computed up to index k */
static void ensure_fib_upto(idx_t k)
{
    if (fib_len > k) return;

    /* Initial allocation */
    if (fib_cache == NULL) {
        fib_len = 2;
        fib_cap = 2;
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) return; /* allocation failure – caller will get UB */
        fib_cache[0] = 0;
        fib_cache[1] = 1;
    }

    /* Grow as needed and compute missing terms */
    while (fib_len <= k) {
        if (fib_len == fib_cap) {
            fib_cap = fib_cap ? fib_cap * 2 : 2;
            fib_cache = (seq_t *)realloc(fib_cache, fib_cap * sizeof(seq_t));
            if (!fib_cache) return;
        }
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        ++fib_len;
    }
}

/* Ensure that lucas cache is computed up to index k */
static void ensure_lucas_upto(idx_t k)
{
    if (lucas_len > k) return;

    /* Initial allocation */
    if (lucas_cache == NULL) {
        lucas_len = 2;
        lucas_cap = 2;
        lucas_cache = (seq_t *)malloc(lucas_cap * sizeof(seq_t));
        if (!lucas_cache) return;
        lucas_cache[0] = 2;
        lucas_cache[1] = 1;
    }

    /* Grow as needed and compute missing terms */
    while (lucas_len <= k) {
        if (lucas_len == lucas_cap) {
            lucas_cap = lucas_cap ? lucas_cap * 2 : 2;
            lucas_cache = (seq_t *)realloc(lucas_cache, lucas_cap * sizeof(seq_t));
            if (!lucas_cache) return;
        }
        lucas_cache[lucas_len] = lucas_cache[lucas_len - 1] + lucas_cache[lucas_len - 2];
        ++lucas_len;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if ((n & 1) == 0) {        /* even index: Fibonacci */
        idx_t k = n >> 1;
        ensure_fib_upto(k);
        return fib_cache[k];
    } else {                   /* odd index: Lucas */
        idx_t k = (n - 1) >> 1;
        ensure_lucas_upto(k);
        return lucas_cache[k];
    }
}
