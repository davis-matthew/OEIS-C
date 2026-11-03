/*
 * A356661  a(n) = n! * Sum_{d|n} 1/d^(n/d - 1).
 * https://oeis.org/A356661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 1 / d^(n/d - 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* ----- factorial cache ----- */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cache_size = 0;   /* number of factorials stored */
    static idx_t fact_cache_cap = 0;    /* allocated capacity (>= size) */

    /* initialise cache if needed */
    if (fact_cache == NULL) {
        fact_cache_cap = n + 1;
        fact_cache     = seq_alloc_new_array(fact_cache_cap);
        fact_cache[0]  = 1;   /* 0! = 1 */
        fact_cache_size = 1;
    }
    /* grow cache if required */
    if (n >= fact_cache_cap) {
        size_t new_cap = fact_cache_cap ? fact_cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < fact_cache_size; ++i)
            new_arr[i] = fact_cache[i];
        free_seq_array(fact_cache);
        fact_cache = new_arr;
        fact_cache_cap = new_cap;
    }
    /* compute missing factorials up to n */
    while (fact_cache_size <= n) {
        seq_t prev = fact_cache[fact_cache_size - 1];
        seq_t next = mul_small(prev, (unsigned int)fact_cache_size);
        fact_cache[fact_cache_size] = next;
        ++fact_cache_size;
    }

    seq_t fact_n = fact_cache[n];

    /* ----- enumerate divisors of n ----- */
    enum { MAX_DIVS = 128 };   /* safe for n <= 10000 */
    idx_t divs[MAX_DIVS];
    size_t div_count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[div_count++] = d;
            if (d != n / d && div_count < MAX_DIVS)
                divs[div_count++] = n / d;
        }
    }

    /* ----- accumulate sum ----- */
    seq_t sum = 0;
    for (size_t i = 0; i < div_count; ++i) {
        idx_t d   = divs[i];
        idx_t exp = n / d - 1;
        seq_t pow = ipow_idx((seq_t)d, exp);
        seq_t term = fact_n / pow;
        sum += term;
    }

    return sum;
}
