/*
 * A252180  Number of length 4+2 0..n arrays with the sum of the maximum minus
 *          the minimum of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A252180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 63, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -a(n-1)+a(n-2)+5*a(n-3)+6*a(n-4)-2*a(n-5)-12*a(n-6)
 *               -16*a(n-7)-3*a(n-8)+17*a(n-9)+25*a(n-10)+13*a(n-11)
 *               -13*a(n-12)-25*a(n-13)-17*a(n-14)+3*a(n-15)+16*a(n-16)
 *               +12*a(n-17)+2*a(n-18)-6*a(n-19)-5*a(n-20)-a(n-21)+a(n-22)+a(n-23)
 * for n > 23, with a(1)…a(23) given below.  The function caches results
 * so each term is computed only once.  It works for any seq_t type
 * (unsigned integer or arbitrary‑precision).  Complexity O(n). */
 
#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[23] = {
    40, 369, 1872, 6361, 17092, 39109, 79672, 148673, 259248, 428045,
    675436, 1027273, 1513112, 2169245, 3037220, 4166229, 5610160,
    7435141, 9709732, 12516761, 15944392, 20095193, 25074784
};

static const int coeff[23] = {
   -1,  1,  5,  6, -2, -12, -16, -3,
   17, 25, 13, -13, -25, -17,   3, 16,
   12,  2, -6, -5, -1,   1,  1
};

static seq_t *cache = NULL;      /* dynamic cache array */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of terms already stored */

/* Grow the cache so that it can hold at least 'n' terms. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 32;
    while ((size_t)n > new_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* OOM */
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Initialise cache with the first 23 terms if it hasn't been done yet. */
static void initialise_cache(void)
{
    if (cache_len >= 23) return;
    memcpy(cache, init_vals, sizeof(init_vals));
    cache_len = 23;
}

/* Compute a(n) for n > 23, assuming cache[0..n-1] is prepared. */
static seq_t compute_recursion(idx_t n)
{
    /* 'n' is 1‑based.  We compute the term at cache[n-1]. */
    __int128 sum = 0;
    idx_t i = n - 1;                   /* index of term to compute */
    for (int k = 0; k < 23; ++k) {
        sum += (__int128)coeff[k] * (__int128)cache[i - 1 - k];
    }
    return (seq_t)sum;                 /* sequence guarantees non‑negative */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1; return 0 */

    if (n <= 23) return init_vals[n - 1];

    ensure_cache_capacity(n);
    initialise_cache();                /* ensure first 23 terms are stored */

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] = compute_recursion(i + 1);
    }
    cache_len = n;

    return cache[n - 1];
}
