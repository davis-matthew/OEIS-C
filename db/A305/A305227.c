/*
 * A305227  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A305227:
   a(1) = 16, a(2) = 120, a(3) = 108, a(4) = 253, a(5) = 757,
   a(6) = 2050, a(7) = 5958, a(8) = 17002, a(9) = 49275,
   a(10) = 142734, a(11) = 414039, a(12) = 1202260,
   a(13) = 3491206, a(14) = 10141687, a(15) = 29461836,
   a(16) = 85594654
   For n > 16:
     a(n) = 4*a(n-1) - a(n-2) - 7*a(n-3) + 5*a(n-5) + 7*a(n-7)
            - 4*a(n-8) - 3*a(n-9) - a(n-10) + a(n-12)
   Computes values for any n ≥ 1 (memoised). */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] holds a(1), etc. */
static idx_t cache_size = 0;  /* number of terms currently cached */

/* First sixteen terms of the sequence, zero‑based for cache index */
static const seq_t init_vals[16] = {
    16ULL,   /* a(1) */
    120ULL,  /* a(2) */
    108ULL,  /* a(3) */
    253ULL,  /* a(4) */
    757ULL,  /* a(5) */
    2050ULL, /* a(6) */
    5958ULL, /* a(7) */
    17002ULL,/* a(8) */
    49275ULL,/* a(9) */
    142734ULL,/* a(10) */
    414039ULL,/* a(11) */
    1202260ULL,/* a(12) */
    3491206ULL,/* a(13) */
    10141687ULL,/* a(14) */
    29461836ULL,/* a(15) */
    85594654ULL /* a(16) */
};

seq_t a(idx_t n) {
    /* n is 1‑based index, n >= 1 */
    if (n == 0) return 0; /* undefined, safe return */

    /* Initial allocation: store the first 16 terms
       to allow memoisation for any n. */
    if (cache == NULL) {
        cache_size = (n > 16) ? n : 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
    }

    /* Grow cache if needed */
    if (n > cache_size) {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        /* Compute new terms from cache_size up to n-1 (zero‑based) */
        for (idx_t i = cache_size; i < n; ++i) {
            idx_t idx = i + 1;              /* real sequence index */
            if (idx <= 16) {
                /* already stored */
                continue;
            }
            seq_t val = 4 * cache[idx - 2];   /* 4*a(n-1) */
            val -= cache[idx - 3];           /* -a(n-2) */
            val -= 7 * cache[idx - 4];       /* -7*a(n-3) */
            val += 5 * cache[idx - 6];       /* +5*a(n-5) */
            val += 7 * cache[idx - 8];       /* +7*a(n-7) */
            val -= 4 * cache[idx - 9];       /* -4*a(n-8) */
            val -= 3 * cache[idx -10];       /* -3*a(n-9) */
            val -= cache[idx -11];           /* -a(n-10) */
            val += cache[idx -13];           /* +a(n-12) */
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
