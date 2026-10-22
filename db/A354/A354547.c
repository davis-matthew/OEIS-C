/*
 * A354547  Least number k <= n such that sopfr(k) = sopfr(n).
 * https://oeis.org/A354547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≤ n such that sopfr(k) = sopfr(n), where sopfr(m)
 *            = Σ p·e for the prime power factorization of m
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exercise:  the function is called as seq_t a(idx_t n) with
   external linkage.  It works for any seq_t and idx_t supplied
   by the build – here idx_t is an unsigned integer type
   (uint64_t) and seq_t is an unsigned integer type or an
   arbitrary‑precision integer class.  The implementation uses
   a simple sieve to compute the smallest prime factor (spf)
   array, then builds the sequence on demand.  A cache of
   previously computed terms is maintained so that calls
   are fast regardless of order. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#  include <cstddef>
#  include <cstdint>
#  include <cstdlib>
#endif

/* Forward declaration required by the build */
seq_t a(idx_t n);

/* ---  Internal state  --- */
static idx_t *spf = NULL;            /* spf[i] = smallest prime dividing i */
static idx_t spf_len = 0;             /* length of spf array */

static seq_t *a_cache = NULL;        /* a_cache[i] holds a(i) for 1 ≤ i ≤ a_cache_alloc-1 */
static idx_t a_cache_len = 0;        /* highest index computed in a_cache */
static idx_t a_cache_alloc = 0;      /* current allocated size of a_cache */

static idx_t *first_occ = NULL;      /* first_occ[s] = first index with sopfr = s */
static idx_t first_occ_alloc = 0;    /* allocated size of first_occ array */

/* ---  Allocation helpers for C++ vs C  --- */
static seq_t *alloc_seq_cache(size_t size)
{
# if defined(__cplusplus)
    return new seq_t[size];
# else
    return (seq_t*)malloc(size * sizeof(seq_t));
# endif
}

static void free_seq_cache(seq_t *p)
{
# if defined(__cplusplus)
    delete [] p;
# else
    free(p);
# endif
}

/* Ensure spf array contains the smallest prime factor for
   every integer up to new_n.  If already long enough, nothing
   is done. */
static void ensure_spf(idx_t new_n)
{
    if (new_n <= spf_len)
        return;

    uint64_t *new_spf = (uint64_t*)malloc((new_n + 1) * sizeof(uint64_t));
    if (!new_spf)
        return;  /* out of memory – caller will handle failure */

    for (idx_t i = 0; i <= new_n; ++i)
        new_spf[i] = i;

    for (idx_t i = 2; (idx_t)i * (idx_t)i <= new_n; ++i)
    {
        if (new_spf[i] == i)
        {
            for (idx_t j = i * i; j <= new_n; j += i)
                if (new_spf[j] == (uint64_t)j)
                    new_spf[j] = i;
        }
    }

    free(spf);
    spf = new_spf;
    spf_len = new_n;
}

/* Ensure first_occ array is large enough to index sum. On
   growth only the newly allocated part is zeroed. */
static void ensure_first_occ(idx_t sum)
{
    if (sum < first_occ_alloc)
        return;

    idx_t new_alloc = sum + 1;
    if (new_alloc < first_occ_alloc * 2)
        new_alloc = first_occ_alloc * 2;
    if (new_alloc == 0) new_alloc = 1;

    idx_t *new_first = (idx_t*)realloc(first_occ, new_alloc * sizeof(idx_t));
    if (!new_first)
        return;  /* out of memory – caller will handle failure */

    /* zero the new portion */
    for (idx_t i = first_occ_alloc; i < new_alloc; ++i)
        new_first[i] = 0;

    first_occ = new_first;
    first_occ_alloc = new_alloc;
}

/* Compute sopfr of m using the spf table.  m is assumed
   to be ≤ spf_len.  The result fits in idx_t for the
   intended limits of this problem. */
static idx_t compute_sopfr(idx_t m)
{
    idx_t sum = 0;
    while (m > 1)
    {
        uint64_t p = spf[m];
        idx_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        sum += (idx_t)p * e;
    }
    return sum;
}

/* Recompute all sequence entries up to and including new_n.
   Missing intermediate steps are computed incrementally. */
static void recompute(idx_t new_n)
{
    if (new_n <= a_cache_len)
        return;

    ensure_spf(new_n);

    /* Expand a_cache if necessary */
    if (new_n + 1 > a_cache_alloc)
    {
        seq_t *new_cache = alloc_seq_cache(new_n + 1);
        if (!new_cache)
            return; /* allocation failure */

        /* Copy existing values */
        if (a_cache)
        {
            for (idx_t i = 1; i <= a_cache_len; ++i)
                new_cache[i] = a_cache[i];
            free_seq_cache(a_cache);
        }
        a_cache = new_cache;
        a_cache_alloc = new_n + 1;
    }

    /* Compute terms from a_cache_len+1 up to new_n */
    for (idx_t i = a_cache_len + 1; i <= new_n; ++i)
    {
        idx_t sop = compute_sopfr(i);
        ensure_first_occ(sop);
        if (first_occ[sop] == 0)
            first_occ[sop] = i;      /* record first occurrence */

        a_cache[i] = (seq_t)first_occ[sop]; /* a(i) is first such k */
    }

    a_cache_len = new_n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* value for n < 1 is irrelevant */

    if (n > a_cache_len)
        recompute(n);

    return a_cache[n];
}
