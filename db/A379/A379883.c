/*
 * A379883  a(1) = 1. Let j = a(n-1) and r = A046144(j). Then for n > 1, if j
 *          is novel and r > 0, a(n) = r. If j is novel and r = 0 then a(n) =
 *          1. If j has occurred k (>1) times already then a(n) = k*j.
 * https://oeis.org/A379883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379883
   Definition: a(1)=1.  For n>1 let j=a(n-1) and r=A046144(j)
   (number of primitive roots of j).  Then
     • if j has appeared only once before (novel) and r>0  → a(n)=r
     • if j is novel and r=0                              → a(n)=1
     • if j has appeared k>1 times                       → a(n)=k*j
   Primitive roots exist for n = 1, 2, 4, p^k, or 2p^k
   with odd prime p.  A046144(1)=1, A046144(2)=1, A046144(4)=1,
   A046144(p^k)=φ(φ(p^k)), A046144(2p^k)=φ(φ(p^k)).
   The routine below memoises the sequence up to the requested index
   and counts occurrences of already generated values.  It is
   written to work with seq_t of any integral size (including
   arbitrary‑precision) and to compile under both C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---- helper: prime factorisation for seq_t (small numbers) ---- */
typedef struct { seq_t prime; uint32_t exp; } factor_t;

static size_t factor_small(seq_t n, factor_t *fac, size_t maxf)
{
    size_t count = 0;
    seq_t t = n;
    uint32_t e = 0;
    /* factor 2 */
    while ((t % (seq_t)2) == 0) {
        t /= (seq_t)2;
        ++e;
    }
    if (e) { fac[count].prime = (seq_t)2; fac[count].exp = e; ++count; }
    /* odd factors up to sqrt(t) */
    seq_t d = (seq_t)3;
    while ((seq_t)d * (seq_t)d <= t && count < maxf) {
        e = 0;
        while (t % d == 0) { t /= d; ++e; }
        if (e) { fac[count].prime = d; fac[count].exp = e; ++count; }
        d += (seq_t)2;
    }
    if (t > 1 && count < maxf) {
        fac[count].prime = t;
        fac[count].exp   = 1;
        ++count;
    }
    return count;
}

/* ---- helper: Euler's totient using factor_small ---- */
static seq_t phi_calc(seq_t m)
{
    if (m <= (seq_t)1) return m;          /* φ(0)=0, φ(1)=1 */
    factor_t fac[32];
    size_t fc = factor_small(m, fac, 32);
    seq_t res = m;
    for (size_t i = 0; i < fc; ++i) {
        seq_t p = fac[i].prime;
        res = res / p * (p - (seq_t)1);
    }
    return res;
}

/* ---- helper: does n have primitive roots? (n>1) ---- */
static bool has_primitive_roots(seq_t n)
{
    if (n == (seq_t)1) return false;  /* handled separately */
    factor_t fac[32];
    size_t fc = factor_small(n, fac, 32);
    if (fc == 1) {                     /* n = p^k */
        seq_t p = fac[0].prime;
        uint32_t e = fac[0].exp;
        if (p == (seq_t)2)
            return (e == 1 || e == 2); /* 2 or 4 */
        return true;                   /* odd prime power */
    }
    if (fc == 2) {                     /* n = 2 * p^k */
        bool seen_two = false;
        seq_t other = 0;
        for (size_t i = 0; i < 2; ++i) {
            if (fac[i].prime == (seq_t)2 && fac[i].exp == 1) {
                seen_two = true;
            } else {
                other = fac[i].prime;
            }
        }
        return seen_two && other != (seq_t)2;
    }
    return false;                      /* more than 2 distinct primes */
}

/* ---- storage for occurrence counts (at most 20000 distinct values) ---- */
#define MAX_DISTINCT 20000
static seq_t seen_vals[MAX_DISTINCT];
static uint32_t seen_cnt[MAX_DISTINCT];
static size_t seen_len = 0;

static uint32_t *find_occurrence(seq_t v)
{
    for (size_t i = 0; i < seen_len; ++i) {
        if (seen_vals[i] == v) return &seen_cnt[i];
    }
    return NULL;
}

static void register_value(seq_t v)
{
    uint32_t *p = find_occurrence(v);
    if (p) {
        ++(*p);
    } else {
        if (seen_len >= MAX_DISTINCT) {
            /* should not happen for n <= 10000 */
            return;
        }
        seen_vals[seen_len] = v;
        seen_cnt[seen_len] = 1;
        ++seen_len;
    }
}

/* ---- sequence computation ---- */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* undefined, safe */
    if ((size_t)n > cache_len) {
        /* grow cache */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
        if (cache_len) memcpy(new_cache, seq_cache, sizeof(seq_t) * cache_len);
        free(seq_cache);
        seq_cache = new_cache;
        /* initialise counters for new elements */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            if (i == 0) {
                seq_cache[0] = (seq_t)1;
                register_value((seq_t)1);
                continue;
            }
            seq_t j = seq_cache[i - 1];
            uint32_t *occ = find_occurrence(j);
            uint32_t k = (occ ? *occ : 0);     /* should be >=1 */
            bool novel = (k == 1);
            seq_t next;
            if (novel) {
                seq_t r;
                if (j == (seq_t)1) {
                    r = (seq_t)1;              /* A046144(1)=1 */
                } else if (has_primitive_roots(j)) {
                    seq_t phi_j = phi_calc(j);
                    r = phi_calc(phi_j);
                } else {
                    r = (seq_t)0;
                }
                if (r > (seq_t)0) next = r;
                else              next = (seq_t)1;
            } else {
                next = (seq_t)k * j;
            }
            seq_cache[i] = next;
            register_value(next);
        }
        cache_len = (size_t)n;
    }
    return seq_cache[n - 1];
}
