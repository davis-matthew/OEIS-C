/*
 * A280602  Number of nX6 0..1 arrays with no element equal to more than two
 *          of its horizontal, vertical and antidiagonal neighbors and with
 *          new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A280602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) – 8*a(n-2) + 4*a(n-3) + 2*a(n-7) – 2*a(n-8) –
 *            3*a(n-9) + 5*a(n-10) – a(n-11) – a(n-12) – a(n-14) + a(n-15) for
 *            n > 19 with the initial values listed in the OEIS entry (a(1) …
 *            a(19))
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 23, u64 n <= 51, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280602 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* defined only for n >= 1 */
        return 0;

    /* --------------------------------------------------------------------- */
    /*  Initial 19 terms (1‑based indexing)                                 */
    /* --------------------------------------------------------------------- */
    static const seq_t init[19] = {
        32ULL, 383ULL, 735ULL, 1709ULL, 3647ULL,
        7594ULL, 15792ULL, 33136ULL, 70293ULL, 150510ULL,
        324417ULL, 702417ULL, 1525112ULL, 3317003ULL,
        7221478ULL, 15731170ULL, 34280316ULL, 74716385ULL,
        162869145ULL
    };

    /* --------------------------------------------------------------------- */
    /*  Static cache for memoisation                                         */
    /* --------------------------------------------------------------------- */
    static seq_t *cache     = NULL;   /* array of computed terms */
    static size_t cache_len = 0;      /* number of terms stored (initially 19) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /*  Initialise cache once ------------------------------------------------ */
    if (!cache) {
        cache_cap = 64;                /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;          /* out‑of‑memory guard */

        for (size_t i = 0; i < 19; ++i)
            cache[i] = init[i];

        cache_len = 19;                /* terms a(1)…a(19) stored */
    }

    /*  Return term if already computed ------------------------------------- */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /*  Grow cache if needed ------------------------------------------------- */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;              /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;            /* out‑of‑memory guard */
        cache = tmp;
        cache_cap = new_cap;
    }

    /*  Compute terms from cache_len+1 up to n ------------------------------ */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to term a(i+1) -> stored at cache[i]              */
        seq_t val = 0;
        val = val + 5 * cache[i - 1];   /* 5*a(n-1) */
        val = val - 8 * cache[i - 2];   /* -8*a(n-2) */
        val = val + 4 * cache[i - 3];   /* +4*a(n-3) */
        val = val + 2 * cache[i - 7];   /* +2*a(n-7) */
        val = val - 2 * cache[i - 8];   /* -2*a(n-8) */
        val = val - 3 * cache[i - 9];   /* -3*a(n-9) */
        val = val + 5 * cache[i - 10];  /* +5*a(n-10) */
        val = val - 1 * cache[i - 11];  /* -a(n-11) */
        val = val - 1 * cache[i - 12];  /* -a(n-12) */
        val = val - 1 * cache[i - 14];  /* -a(n-14) */
        val = val + 1 * cache[i - 15];  /* +a(n-15) */

        cache[i] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
