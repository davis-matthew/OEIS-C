/*
 * A367363  First term is 1; thereafter, if u and v are consecutive terms,
 *          with decimal expansions u = bc...ef, v = hi...jk, then v-u has
 *          decimal expansion efhi, formed by concatenating the last two
 *          digits of u and the first two digits of v. If there is a choice
 *          for v, pick the smallest.
 * https://oeis.org/A367363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*OEIS A367363
  Recurrence: a(1)=1.
  For n>1, let u=a(n-1).  Let ef be the last two digits of u.
  Search the smallest integer f (0≤f≤99) such that
     v = u + ef·100 + f
  has first two decimal digits equal to f.
  Then a(n)=v.  The sequence is memoised so that calls in any
  order run in O(n) time for n up to ≈10000. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers for all builds -------------------------------- */
static int digit_len(seq_t v)
/* number of decimal digits of v (v>0) */
{
    int l = 0;
    seq_t t = v;
    do {
        ++l;
        t = t / (seq_t)10;
    } while (t != 0);
    return l;
}

static seq_t pow10_seq(idx_t e)
/* return 10^e as seq_t */
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) r *= (seq_t)10;
    return r;
}

static seq_t next_term(seq_t u)
/* compute the next term after u */
{
    seq_t last2 = u % (seq_t)100;          /* ef */
    seq_t base  = u + last2 * (seq_t)100;   /* u + ef·100 */
    for (int f = 0; f < 100; ++f) {
        seq_t v = base + (seq_t)f;
        int    len = digit_len(v);
        seq_t  first_two;
        if (len >= 2) {
            seq_t pw = pow10_seq((idx_t)(len - 2));
            first_two = v / pw;
        } else {
            first_two = v;   /* v has one digit */
        }
        if (first_two == (seq_t)f) return v;
    }
    /* Theoretically never reached */
    return base;
}

/* ---------- C++ path --------------------------------------------- */
#ifdef __cplusplus
#include <vector>

static std::vector<seq_t> cache_vec;
/* The sequence is stored in cache_vec.  Elements are 0‑based
   corresponding to a(1), a(2), … */

static inline void ensure_cache(idx_t n)
/* grow cache up to index n (1‑based) */
{
    while (cache_vec.size() < n) {
        if (cache_vec.empty())
            cache_vec.push_back((seq_t)1);        /* a(1) = 1 */
        else
            cache_vec.push_back(next_term(cache_vec.back()));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused offset */
    ensure_cache(n);
    return cache_vec[n-1];
}

#else /* ---------- C path ------------------------------------------- */

static seq_t *seq_cache = NULL;          /* dynamic array of terms */
static idx_t   cache_len = 0;            /* number of stored terms */
static idx_t   cache_cap = 0;            /* allocated capacity */

static void ensure_seq_capacity(idx_t needed)
/* allocate or grow the array so it can hold at least 'needed' elements */
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    cache_cap = new_cap;
}

static void append_term(seq_t val)
/* store a new term in the cache */
{
    ensure_seq_capacity(cache_len + 1);
    seq_cache[cache_len++] = val;
}

static inline void ensure_cache(idx_t n)
/* make sure terms up to n (1‑based) are computed */
{
    while (cache_len < n) {
        if (cache_len == 0)
            append_term((seq_t)1);              /* a(1) = 1 */
        else
            append_term(next_term(seq_cache[cache_len-1]));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused offset */
    ensure_cache(n);
    return seq_cache[n-1];
}

#endif
