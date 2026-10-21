/*
 * A281467  Number of nX6 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A281467 – number of n×6 0..1 arrays with no element having
    more than two of its horizontal, diagonal or antidiagonal
    neighbours equal, and with new values introduced in order 0
    sequentially upwards.
    Recurrence (valid for n>17):
        a(n) = 2*a(n-1) + 3*a(n-3) - 4*a(n-4) - 4*a(n-5)
               + a(n-6) - a(n-7) + 2*a(n-8) + 5*a(n-9) - 3*a(n-11)
    Initial values for 1 ≤ n ≤ 17 (computed by the definition) are:
        a(1)=32, a(2)=169, a(3)=87, a(4)=176, a(5)=358,
        a(6)=706, a(7)=1422, a(8)=2858, a(9)=5793,
        a(10)=11986, a(11)=24812, a(12)=51020,
        a(13)=105433, a(14)=218415, a(15)=451445,
        a(16)=933860, a(17)=1933761.
*/

#include <stdlib.h>           /* calloc, realloc, abort */

/* --------- memoisation storage ------------------------------------- */
static seq_t *cache = NULL;   /* array of computed terms a(0)...a(n) */
static size_t size  = 0;      /* number of initialised entries */
static size_t cap   = 0;      /* allocated capacity of cache */

/* --------- initial eight values (for 1≤n≤17) ----------------------- */
static const seq_t init[18] = {
    0,           /* unused a(0) */
    32ULL,        /* a(1) */
    169ULL,       /* a(2) */
    87ULL,        /* a(3) */
    176ULL,       /* a(4) */
    358ULL,       /* a(5) */
    706ULL,       /* a(6) */
    1422ULL,      /* a(7) */
    2858ULL,      /* a(8) */
    5793ULL,      /* a(9) */
    11986ULL,     /* a(10) */
    24812ULL,     /* a(11) */
    51020ULL,     /* a(12) */
    105433ULL,    /* a(13) */
    218415ULL,    /* a(14) */
    451445ULL,    /* a(15) */
    933860ULL,    /* a(16) */
    1933761ULL    /* a(17) */
};

/* --------- ensure at least “need” capacity in cache ---------------- */
static void ensure_cache_cap(size_t need)
{
    if (cap >= need) return;
    size_t new_cap = cap ? cap << 1 : 32;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* --------- main sequence function ---------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* invalid index */
    size_t N = (size_t)n;                       /* safe conversion, n≤UINT64_MAX */

    /* --- handle initial values ----------------------------------- */
    if (N <= 17) return init[N];

    /* --- initialise cache on first call or when size==0 ----- */
    if (!cache) {
        /* allocate for first 18 terms at least */
        cap   = N + 1 > 18 ? N + 1 : 18;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) abort();                    /* allocation failure */
        for (size_t i = 1; i <= 17; ++i) cache[i] = init[i];
        size = 18;                              /* entries 0..17 are valid */
    }

    /* --- extend cache if needed ------------------------------- */
    if (size <= N) {
        ensure_cache_cap(N + 1);
        for (size_t i = size; i <= N; ++i) {
            /* compute a(i) by the recurrence */
            seq_t pos = (cache[i-1] << 1)          /* 2*a(i-1) */
                      + (cache[i-3] * (seq_t)3)    /* +3*a(i-3) */
                      + cache[i-6];               /* + a(i-6) */
            pos += (cache[i-8] << 1);             /* +2*a(i-8) */
            pos += (cache[i-9] * (seq_t)5);       /* +5*a(i-9) */

            seq_t neg = (cache[i-4] * (seq_t)4)    /* +4*a(i-4) */
                      + (cache[i-5] * (seq_t)4)    /* +4*a(i-5) */
                      + cache[i-7];               /* + a(i-7) */
            neg += (cache[i-11] * (seq_t)3);      /* +3*a(i-11) */

            cache[i] = pos - neg;                /* a(i) = pos – neg */
        }
        size = N + 1;                           /* now all up to N are valid */
    }

    return cache[N];
}
