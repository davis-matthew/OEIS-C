/*
 * A364173  a(n) = (9*n)!*(2*n)!*(3*n/2)!/((9*n/2)!*(4*n)!*(3*n)!*n!).
 * https://oeis.org/A364173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (9*n)!*(2*n)!*(3*n/2)!/((9*n/2)!*(4*n)!*(3*n)!*n!)
 * Recurrence: a(n) = 108*(9n-1)*(9n-5)*(9n-7)*(9n-11)*(9n-13)*(9n-17)
/*                /(n*(n-1)*(4n-1)*(4n-3)*(4n-5)*(4n-7))*a(n-2),   a(0)=1, a(1)=128
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static idx_t   cache_max  = 0;

static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        cache_size = 2;
        seq_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (seq_cache == NULL) return;           /* out of memory; caller will fail */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)128;
        cache_max = 1;
    }

    if (n <= cache_max) return;

    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return;                /* out of memory; caller will fail */
        seq_cache = tmp;
        cache_size = new_size;
    }

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i < 2) continue;   /* base cases already set */

        if (sizeof(seq_t) <= 8) {   /* 64‑bit or smaller: use 128‑bit intermediate */
            unsigned __int128 num = 108;
            unsigned __int128 den = 1;
            idx_t ni = i;
            num *= (unsigned __int128)(9*ni-1);
            num *= (unsigned __int128)(9*ni-5);
            num *= (unsigned __int128)(9*ni-7);
            num *= (unsigned __int128)(9*ni-11);
            num *= (unsigned __int128)(9*ni-13);
            num *= (unsigned __int128)(9*ni-17);

            den *= (unsigned __int128)ni;
            den *= (unsigned __int128)(ni-1);
            den *= (unsigned __int128)(4*ni-1);
            den *= (unsigned __int128)(4*ni-3);
            den *= (unsigned __int128)(4*ni-5);
            den *= (unsigned __int128)(4*ni-7);

            unsigned __int128 val = (unsigned __int128)seq_cache[i-2];
            unsigned __int128 prod = val * num;
            prod /= den;
            seq_cache[i] = (seq_t)prod;
        } else {   /* arbitrary‑precision or >64‑bit: rely on seq_t operations */
            seq_t num = 108;
            seq_t den = 1;
            idx_t ni = i;
            num = num * (seq_t)(9*ni-1);
            num = num * (seq_t)(9*ni-5);
            num = num * (seq_t)(9*ni-7);
            num = num * (seq_t)(9*ni-11);
            num = num * (seq_t)(9*ni-13);
            num = num * (seq_t)(9*ni-17);

            den = den * (seq_t)ni;
            den = den * (seq_t)(ni-1);
            den = den * (seq_t)(4*ni-1);
            den = den * (seq_t)(4*ni-3);
            den = den * (seq_t)(4*ni-5);
            den = den * (seq_t)(4*ni-7);

            seq_t prod = seq_cache[i-2] * num;
            seq_cache[i] = prod / den;   /* exact division by construction */
        }
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= cache_max) return seq_cache[n];
    /* on failure (e.g. out of memory) return 0 to signal error */
    return (seq_t)0;
}
