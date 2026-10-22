/*
 * A368469  a(n) is the sum of exponentially odd divisors of the n-th
 *          exponentially odd number.
 * https://oeis.org/A368469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A368469: a(n) = sum of exponentially odd divisors of the
   n-th exponentially odd number (offset 1). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Cache for computed terms.  cache[0] holds a(1), cache[1] holds a(2), ... */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms currently stored        */
static size_t cache_cap  = 0;   /* allocated capacity of cache             */
/* next integer to test for being an exponentially odd number */
static uint64_t cur_val = 1;

/* Ensure cache has room for at least 'need' elements.  Reallocs as necessary. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* fatal if memory exhausted */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Sum of exponentially odd divisors of the integer cur_val.
   Returns 0 if cur_val is not an exponentially odd number.  */
static seq_t sum_expo_div(void)
{
    uint64_t n = cur_val;

    if (n == 1)
        return (seq_t)1;   /* 1 is treated as an exponentially odd number */

    uint64_t tmp = n;
    seq_t prod = (seq_t)1;

    while (tmp > 1) {
        /* Smallest prime factor by trial division (fast enough for n≤~10⁵). */
        uint64_t p = 2;
        while (p * p <= tmp && tmp % p != 0)
            ++p;
        if (p * p > tmp)
            p = tmp;

        /* Collect exponent of p. */
        uint64_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        if ((exp & 1ULL) == 0)      /* even exponent ⇒ not exponent‑odd */
            return (seq_t)0;

        /* Sum of odd powers of p: p  + p³ + … + p^exp (exp odd). */
        seq_t p_seq = (seq_t)p;
        seq_t term  = p_seq;           /* current odd power, starting at p¹ */
        seq_t sum_p = (seq_t)0;
        uint64_t cnt = (exp + 1) >> 1; /* number of odd exponents */
        for (uint64_t i = 0; i < cnt; ++i) {
            sum_p += term;
            term   *= p_seq * p_seq;    /* next odd power => multiply by p² */
        }

        prod *= ((seq_t)1 + sum_p);     /* include the choice of not using p */
    }

    return prod;
}

/* Generate terms until the cache contains at least 'n' entries. */
static void generate_upto(idx_t n)
{
    while (cache_size < (size_t)n) {
        seq_t val = sum_expo_div();
        cur_val++;                    /* advance to next integer */

        if (val != (seq_t)0) {         /* store only exponent‑odd numbers */
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = val;
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n<1 (unused) */
    generate_upto(n);
    return cache[(size_t)n - 1];
}
