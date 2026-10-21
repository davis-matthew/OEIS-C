/*
 * A288917  Number of 6-cycles in the n X n queen graph.
 * https://oeis.org/A288917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=3*a(n-2)+2*a(n-3)-a(n-4)-5*a(n-5)-6*a(n-6)-a(n-7)+6*a(n-8)+11*a(n-9)
 *            +6*a(n-10)-4*a(n-11)-10*a(n-12)-10*a(n-13)-4*a(n-14)
 *            +6*a(n-15)+11*a(n-16)+6*a(n-17)-a(n-18)-6*a(n-19)-5*a(n-20)-a(n-21)
 *            +2*a(n-22)+3*a(n-23)-a(n-25), a(0)=0. Initial values for n=1..23
 *            are given below
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const int coeff[26] = {
    0,   /* 0    */ 0,   /* 1    */
    3,   /* 2    */ 2,   /* 3    */ -1,   /* 4    */ -5,   /* 5    */ -6,   /* 6    */
    -1,   /* 7    */ 6,   /* 8    */ 11,   /* 9    */ 6,   /*10    */ -4,   /*11 */
    -10,  /*12    */ -10, /*13    */ -4,   /*14    */ 6,   /*15    */ 11,   /*16 */
    6,    /*17    */ -1,   /*18    */ -6,   /*19    */ -5,   /*20    */ -1,   /*21 */
    2,    /*22    */ 3,   /*23    */ 0,    /*24    */ -1   /*25    */
};

static const seq_t init_vals[24] = {
    0ULL,  /* a(0) */
    0ULL,  /* a(1) */
    0ULL,  /* a(2) */
    976ULL,                 /* a(3) */
    26414ULL,               /* a(4) */
    207136ULL,              /* a(5) */
    951750ULL,              /* a(6) */
    3190304ULL,             /* a(7) */
    8721576ULL,             /* a(8) */
    20615340ULL,            /* a(9) */
    43736788ULL,            /* a(10) */
    85332868ULL,            /* a(11) */
    155691014ULL,           /* a(12) */
    268995384ULL,           /* a(13) */
    444000894ULL,           /* a(14) */
    705336724ULL,           /* a(15) */
    1083967932ULL,          /* a(16) */
    1619250600ULL,          /* a(17) */
    2358809948ULL,          /* a(18) */
    3361809572ULL,          /* a(19) */
    4697700286ULL,          /* a(20) */
    6451359236ULL,          /* a(21) */
    8719953330ULL,          /* a(22) */
    11620829368ULL          /* a(23) */
};

/* Compute a single term using the recurrence (coefficients from coeff array) */
static seq_t compute_term(idx_t i, seq_t *cache) {
    signed __int128 total = 0;   /* intermediate to avoid overflow */
    for (int k = 2; k <= 25; ++k) {
        int c = coeff[k];
        if (c == 0) continue;
        if (i < (idx_t)k) continue;          /* negative index -> skip */
        idx_t j = i - k;
        unsigned __int128 prod = (unsigned __int128)cache[j]
                                 * (unsigned __int128)(c > 0 ? c : -c);
        if (c > 0)
            total += (signed __int128)prod;
        else
            total -= (signed __int128)prod;
    }
    return (seq_t)total;
}

seq_t a(idx_t n) {
    /* Lazy memoization */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;

    if (n <= 23)              /* direct return for known terms */
        return init_vals[n];

    if (!cache) {              /* first allocation */
        size_t init_size = 26;          /* indices 0..25 inclusive */
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i <= 23; ++i)
            cache[i] = init_vals[i];
        /* a(24) and a(25) are derived from the recurrence using a(0)=0 */
        cache[24] = compute_term(24, cache);
        cache[25] = compute_term(25, cache);
        max_cached = 25;
    }

    if (n <= max_cached)
        return cache[n];

    /* Grow cache to accommodate up to n */
    size_t needed = (size_t)n + 1;          /* indices 0..n */
    if (needed > (size_t)max_cached + 1) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms from max_cached+1 up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
        cache[i] = compute_term(i, cache);
    max_cached = n;

    return cache[n];
}
