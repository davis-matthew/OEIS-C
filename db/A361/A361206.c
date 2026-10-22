/*
 * A361206  Lexicographically earliest infinite sequence of distinct imperfect
 *          numbers such that the sum of the abundance of all terms is never <
 *          1.
 * https://oeis.org/A361206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = lexicographically earliest imperfect number such that
 *              the cumulative sum of abundances (σ(x)‑2x) of all selected
 *              terms remains never < 1.  Imperfect numbers are all
 *              positive integers that are not perfect.
 *
 * The implementation constructs the sequence greedily, keeping an
 * auxiliary array of already-used candidates and a cache of
 * abundances.  It can be called with any order of indices and works
 * for all supported seq_t and idx_t types.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/* --------------- local storage ---------------------------------- */
static seq_t *seq = NULL;          /* constructed sequence values          */
static size_t seq_cap = 0;         /* capacity of seq array                */
static idx_t  seq_len = 0;         /* number of values already built       */

static unsigned char *used = NULL; /* flag array: 1 = already used         */
static size_t used_len = 0;        /* number of bytes in used array        */

static long long *ab_cache = NULL; /* cached abundances (σ(n)-2n)          */
static size_t ab_len = 0;          /* size of ab_cache array               */

/* --------------- auxiliary helpers ------------------------------ */

static void
ensure_seq_capacity(idx_t min_cap)
{
    if ((size_t)min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)min_cap)
        new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void
ensure_used_capacity(idx_t max_index)
{
    if ((size_t)max_index < used_len)
        return;
    size_t new_len = used_len ? used_len * 2 : 8;
    while (new_len <= (size_t)max_index)
        new_len <<= 1;
    used = (unsigned char *)realloc(used, new_len * sizeof(unsigned char));
    for (size_t i = used_len; i < new_len; ++i)
        used[i] = 0;
    used_len = new_len;
}

static void
ensure_ab_capacity(idx_t max_index)
{
    if ((size_t)max_index < ab_len)
        return;
    size_t new_len = ab_len ? ab_len * 2 : 8;
    while (new_len <= (size_t)max_index)
        new_len <<= 1;
    long long *new_ab = (long long *)realloc(ab_cache, new_len * sizeof(long long));
    for (size_t i = ab_len; i < new_len; ++i)
        new_ab[i] = LLONG_MAX;          /* sentinel for "not computed" */
    ab_cache = new_ab;
    ab_len = new_len;
}

/* sum of divisors σ(n) for n <= 2^64-1 */
static seq_t
sigma_u64(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t power = 1;
            seq_t term = 1;
            while (temp % p == 0)
            {
                temp /= p;
                power *= p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1)
        result *= (seq_t)(1 + temp);
    return result;
}

/* obtain abundance σ(n)-2n, caching the result */
static long long
get_ab(idx_t n)
{
    ensure_ab_capacity(n);
    if (ab_cache[n] != LLONG_MAX)
        return ab_cache[n];
    seq_t sigma = sigma_u64((uint64_t)n);
    long long ab = (long long)sigma - (long long)2 * (long long)n;
    ab_cache[n] = ab;
    return ab;
}

/* ----------------- main function --------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* outside of defined offset */

    static long long acc = 0;        /* cumulative abundance sum */

    /* Build the sequence up to the required length. */
    while (seq_len < n)
    {
        for (idx_t cand = 1; ; ++cand)
        {
            ensure_used_capacity(cand);
            if (used[cand])
                continue;               /* already chosen */

            long long ab = get_ab(cand);
            if (ab == 0)                /* perfect number skip */
                continue;

            if (acc + ab >= 1)          /* admissible, choose it */
            {
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len++] = (seq_t)cand;
                used[cand] = 1;
                acc += ab;
                break;
            }
        }
    }
    return seq[n - 1];
}
