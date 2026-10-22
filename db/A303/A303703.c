/*
 * A303703  Positive numbers that are congruent mod k! to a divisor of k! for
 *          any k > 0.
 * https://oeis.org/A303703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) consists of the positive integers m for which for every
   k > 0, the remainder m mod k! is a divisor of k! (equivalently,
   there exists a divisor d of k! with m ≡ d (mod k!)). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* in the build 'seq_t' is the return type (big integer or integer),
   'idx_t' is an unsigned integer type (uint64_t).  These types are
   defined externally; we only use them here. */

/* ----------------------------------------------------------------- */
/* Factorial table ------------------------------------------------- */
static seq_t *fac = NULL;         /* fac[i] = i!  (fac[0] = 1, fac[1] = 1) */
static size_t fac_cap = 0;        /* number of allocated slots in fac   */
static size_t fac_len = 0;        /* number of factorials currently stored */

/* ----------------------------------------------------------------- */
/* Sequence cache -------------------------------------------------- */
static seq_t *cache = NULL;       /* cache[i] = a(i+1)   (since offset 1) */
static size_t cache_cap = 0;      /* number of allocated cache slots */
static size_t cache_len = 0;      /* number of terms currently stored */
static idx_t next_candidate = 1;  /* next integer to test */

/* ----------------------------------------------------------------- */
/* Ensure that the factorial table has at least 'new_cap' slots. */
static void ensure_factorial_capacity(size_t new_cap)
{
    /* cast to seq_t* for C++ compliance */
    seq_t *new_arr = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    fac = new_arr;
    fac_cap = new_cap;
}

/* Ensure that the cache has at least 'new_cap' slots. */
static void ensure_cache_capacity(size_t new_cap)
{
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* ----------------------------------------------------------------- */
/* Dynamically extend the factorial table until fac[fac_len] > m. */
static void extend_factorials(seq_t m)
{
    /* make sure we have at least one slot for the next factorial */
    while (fac_len == fac_cap) {
        size_t new_cap = fac_cap ? fac_cap * 2 : 8;
        ensure_factorial_capacity(new_cap);
    }

    /* compute factorials until we find the first one exceeding m */
    while (true) {
        /* fac[fac_len] = fac[fac_len-1] * fac_len */
        seq_t next = fac[fac_len - 1] * (seq_t)fac_len;
        fac[fac_len] = next;

        if (next > m) {
            /* fac_len now indexes the first factorial > m */
            break;
        } else {
            fac_len++;
            /* ensure capacity for the next iteration */
            if (fac_len == fac_cap) {
                size_t new_cap = fac_cap ? fac_cap * 2 : 8;
                ensure_factorial_capacity(new_cap);
            }
        }
    }
}

/* ----------------------------------------------------------------- */
/* Test whether a given number m satisfies the defining property.   */
static bool is_valid(seq_t m)
{
    extend_factorials(m);                 /* ensures fac[fac_len] > m    */
    size_t maxK = fac_len - 1;            /* last factorial <= m         */

    /* Check all k such that k! <= m. */
    for (idx_t k = 1; k <= (idx_t)maxK; ++k) {
        seq_t r = m % fac[(size_t)k];
        if (r != 0) {
            if (fac[(size_t)k] % r != 0) return false;
        }
    }

    /* Check the first k with k! > m  (fac[fac_len] is that factorial). */
    if (fac[fac_len] % m != 0) return false;

    return true;
}

/* ----------------------------------------------------------------- */
/* One-time initialization of the static tables.                     */
static void init_tables(void)
{
    if (fac) return;           /* already initialized          */

    /* Factorial table: start with 0! = 1, 1! = 1. */
    fac_cap = 8;
    fac = (seq_t *)malloc(fac_cap * sizeof(seq_t));
    fac[0] = (seq_t)1;
    fac[1] = (seq_t)1;
    fac_len = 2;               /* next free index is 2         */

    /* Cache table is initially empty. */
    cache_cap = 0;
    cache = NULL;
    cache_len = 0;
    next_candidate = 1;
}

/* ----------------------------------------------------------------- */
/* Public function: returns the nth term of the sequence.            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined index, safe guard    */

    init_tables();             /* ensure tables are ready        */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = (seq_t)next_candidate;
        if (is_valid(m)) {
            /* make room in the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                ensure_cache_capacity(new_cap);
            }
            cache[cache_len++] = m;
        }
        ++next_candidate;
    }

    return cache[n - 1];       /* offset 1                        */
}
