/*
 * A325487  One of the four successive approximations up to 13^n for the
 *          13-adic integer 6^(1/4). This is the 4 (mod 5) case (except for n
 *          = 0).
 * https://oeis.org/A325487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique integer k in [1,5^n] congruent to 4 (mod 5) *
 *            such that k^4 ≡ 6 (mod 5^n). a(0)=0. The value is obtained by *
 *            Hensel lifting: starting from the solution modulo 5, each step
 *            lifts * the solution to the next power of 5. The lifting step is
 *            * a_{m+1} = a_m + 5^m * t, t ≡ -( (a_m^4...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mod_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned m)   */
#include "../utils/mod_pow.h"     /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod) */

static seq_t *a_cache = NULL;        /* a(0), a(1), ... */
static size_t a_cap   = 0;
static idx_t  a_len   = 0;

static seq_t *pow_cache = NULL;      /* 5^0, 5^1, 5^2, ... */
static size_t pow_cap   = 0;
static idx_t  pow_len   = 0;

/* Ensure that an array has space for indices 0..n.
   current_len is the length currently stored. */
static void ensure_cache_capacity(idx_t n, seq_t **cache, size_t *cap, size_t current_len)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= *cap) return;

    size_t new_cap = *cap ? *cap : 4;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (*cache) {
        for (size_t i = 0; i < current_len; ++i) new_buf[i] = (*cache)[i];
        free(*cache);
    }
    *cache = new_buf;
    *cap   = new_cap;
}

/* Hensel lifting implementation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (a_len == 0) {          /* first call: initialise caches */
        ensure_cache_capacity(1, &a_cache, &a_cap, 0);
        a_cache[0] = (seq_t)0;          /* a(0) */
        a_cache[1] = (seq_t)4;          /* a(1) */
        a_len = 2;

        ensure_cache_capacity(1, &pow_cache, &pow_cap, 0);
        pow_cache[0] = (seq_t)1;        /* 5^0 */
        pow_cache[1] = (seq_t)5;        /* 5^1 */
        pow_len = 2;
    }

    while (a_len <= n) {       /* lift from a_len-1 up to n */
        idx_t m          = a_len;       /* new index to compute */
        seq_t a_prev     = a_cache[m-1];
        seq_t pow_prev   = pow_cache[m-1];

        seq_t pow_cur = mul_small(pow_prev, 5);   /* 5^m */

        /* store 5^m */
        ensure_cache_capacity(m, &pow_cache, &pow_cap, pow_len);
        pow_cache[pow_len++] = pow_cur;

        /* compute a_m via Hensel formula */
        seq_t pow4 = mod_pow(a_prev, 4, pow_cur);
        seq_t diff = (pow4 >= 6) ? (pow4 - 6) : (pow4 + pow_cur - 6);

        seq_t div = diff / pow_prev;            /* (a_prev^4-6)/5^(m-1)  */
        unsigned t = (unsigned)(div % (seq_t)5);  /* s = div mod 5 */
        t = (5 - t) % 5;                        /* t ≡ -s (mod 5) */

        seq_t new_k = a_prev + mul_small(pow_prev, t);

        /* store new a value */
        ensure_cache_capacity(m, &a_cache, &a_cap, a_len);
        a_cache[a_len++] = new_k;
    }

    return a_cache[n];
}
