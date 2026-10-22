/*
 * A395572  Array read by ascending antidiagonals: A(n, k) = Sum_{j=0..n}
 *          Stirling2(n, j)*4^j*(k)_j.
 * https://oeis.org/A395572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A395572
    a(n) is the array read by ascending antidiagonals of
    A(n,k) = Σ_{j=0..n} Stirling2(n,j) * 4^j * (k)_j .
    The implementation keeps a small table of Stirling numbers and
    powers of 4 (max diagonal needed for n up to 10000 is < 200)
    and lazily expansion the result cache.  It works for any
    seq_t (fixed‑size or arbitrary‑precision) and any idx_t
    (uint64_t).  Allocation is handled separately for C and C++. */

#include <stdlib.h>
#include <stdint.h>

#ifndef MAX_STIRLING
#define MAX_STIRLING 200   /* sufficient for diagonals up to ~10000 elements */
#endif

/* Allocation helpers that work in both C and C++ */
#ifdef __cplusplus
#define ALLOC_SEQ(n)  new seq_t[(n)]
#define FREE_SEQ(p)   delete[] (p)
#else
#define ALLOC_SEQ(n)  ((seq_t*)malloc((n) * sizeof(seq_t)))
#define FREE_SEQ(p)   free(p)
#endif

static seq_t *stirling = NULL;   /* Stirling numbers S(n,k) 0 ≤ n,k ≤ MAX_STIRLING */
static seq_t *pow4     = NULL;   /* powers of 4, 4^j, j ≤ MAX_STIRLING */

static seq_t *cache = NULL;      /* cache of a(n) */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  next_diag = 0;     /* next diagonal to generate */

/* Allocate an array of seq_t of given capacity */
static seq_t* alloc_seq(size_t cap)
{
    return ALLOC_SEQ(cap);
}

/* Free an array of seq_t */
static void free_seq_arr(seq_t *p)
{
    FREE_SEQ(p);
}

/* Grow the cache to at least new_cap */
static void grow_cache(size_t new_cap)
{
    seq_t *newp = alloc_seq(new_cap);
    for (size_t i = 0; i < cache_len; ++i)
        newp[i] = cache[i];
    free_seq_arr(cache);
    cache = newp;
    cache_cap = new_cap;
}

/* Push a new value into the cache, resizing if necessary */
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
        grow_cache(cache_cap == 0 ? 8 : cache_cap * 2);
    cache[cache_len++] = val;
}

/* Compute A(n,k) = Σ_{j=0..n} S(n,j)*4^j*(k)_j */
static seq_t compute_A(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t falling = 1;   /* (k)_0 = 1 */
    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = stirling[n * (MAX_STIRLING + 1) + j];
        term *= pow4[j];
        term *= falling;
        sum += term;
        if (j + 1 <= n) {
            seq_t sub = (seq_t)(k > j ? (k - j) : 0);
            falling *= sub;   /* update (k)_j+1 = (k)_j * (k-j) */
        }
    }
    return sum;
}

/* Initialise Stirling numbers and powers of 4 */
static void init_tables(void)
{
    if (stirling)                /* already initialised */
        return;

    size_t dim = (size_t)(MAX_STIRLING + 1) * (size_t)(MAX_STIRLING + 1);
    stirling = alloc_seq(dim);
    pow4     = alloc_seq(MAX_STIRLING + 1);

    /* base cases */
    stirling[0 * (MAX_STIRLING + 1) + 0] = 1;
    for (size_t i = 1; i <= MAX_STIRLING; ++i)
        stirling[i * (MAX_STIRLING + 1) + i] = 1; /* S(i,i)=1 */

    pow4[0] = 1;
    for (size_t i = 1; i <= MAX_STIRLING; ++i)
        pow4[i] = pow4[i - 1] * 4;

    /* recurrence: S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
    for (size_t n = 1; n <= MAX_STIRLING; ++n) {
        for (size_t k = 1; k <= n; ++k) {
            seq_t val = stirling[(n - 1) * (MAX_STIRLING + 1) + k];
            val *= (seq_t)k;
            val += stirling[(n - 1) * (MAX_STIRLING + 1) + k - 1];
            stirling[n * (MAX_STIRLING + 1) + k] = val;
        }
    }
}

/* Extend the cache up to (and including) target index */
static void extend_cache_to(idx_t target)
{
    init_tables();    /* ensure tables exist */

    while (cache_len <= (size_t)target) {
        /* current diagonal is next_diag */
        /* generate A(n,k) for n = next_diag … 1, with k = next_diag - n */
        for (idx_t n = next_diag; n > 0; --n) {
            idx_t k = next_diag - n;
            seq_t val = compute_A(n, k);
            cache_push(val);
        }
        /* last entry for n = 0 : A(0,k) = 1 */
        cache_push(1);
        ++next_diag;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n >= cache_len)
        extend_cache_to(n);
    return cache[n];
}
