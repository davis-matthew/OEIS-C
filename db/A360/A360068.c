/*
 * A360068  Number of integer partitions of n such that the parts have the
 *          same mean as the multiplicities.
 * https://oeis.org/A360068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360068 – Number of integer partitions of n such that the parts
    have the same mean as the multiplicities.
    This implementation enumerates all admissible numbers of parts k
    (i.e. those with n | k^2) and counts the partitions of n into k
    parts with exactly d = k^2 / n distinct part values using recursion
    with memoisation.  The recursion generates distinct part values in
    strictly increasing order, so no over‑counting occurs.  The cache
    is cleared on each call by bumping a generation counter.  The code
    is compliant with C11 and C++17 and works with the arbitrary‑precision
    seq_t type supplied by the build. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- cache configuration -------------------------------------------------- */
#define HASH_SIZE 1048576U  /* must be a power of two */

static uint64_t cache_key[HASH_SIZE];
static seq_t    cache_val[HASH_SIZE];
static uint32_t cache_used[HASH_SIZE];
static uint32_t cache_generation = 1;

/* ---- helper: build a 64‑bit hash key from the state -------------------- */
static inline uint64_t make_key(uint64_t rem,
                                uint64_t parts,
                                uint64_t distinct,
                                uint64_t minv)
{
    /* each field fits in 16 bits */
    return (rem   << 48) |
           (parts << 32) |
           (distinct << 16) |
           (minv);
}

/* ---- cache lookup ------------------------------------------------------- */
static inline bool cache_lookup(uint64_t key, seq_t *out)
{
    uint32_t idx = (uint32_t)(key ^ (key >> 32)) & (HASH_SIZE - 1);
    while (cache_used[idx] == cache_generation) {
        if (cache_key[idx] == key) {
            *out = cache_val[idx];
            return true;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
    return false;
}

/* ---- cache insert ------------------------------------------------------- */
static inline void cache_insert(uint64_t key, seq_t val)
{
    uint32_t idx = (uint32_t)(key ^ (key >> 32)) & (HASH_SIZE - 1);
    while (cache_used[idx] == cache_generation) {
        if (cache_key[idx] == key) {   /* should not happen */
            cache_val[idx] = val;
            return;
        }
        idx = (idx + 1) & (HASH_SIZE - 1);
    }
    cache_key[idx]  = key;
    cache_val[idx]  = val;
    cache_used[idx] = cache_generation;
}

/* ---- recursive counting of partitions ----------------------------------- */
/* Count the partitions of rem into parts parts with exactly distinct
   distinct values, where the smallest allowed part value is minv.  The
   distinct values are generated in strictly increasing order. */
static seq_t count_partitions(uint64_t rem,
                              uint64_t parts,
                              uint64_t distinct,
                              uint64_t minv)
{
    /* trivial impossibilities */
    if (parts < distinct || parts == 0)
        return 0;
    if (parts == 0)
        return (rem == 0 && distinct == 0) ? 1 : 0;

    /* all remaining parts must be equal */
    if (distinct == 1) {
        if (rem % parts == 0) {
            seq_t v = (seq_t)(rem / parts);
            return (v >= (seq_t)minv) ? 1 : 0;
        }
        return 0;
    }

    /* minimal value too large to fit */
    if (minv > rem / parts)
        return 0;

    /* cache lookup */
    uint64_t key = make_key(rem, parts, distinct, minv);
    seq_t cached;
    if (cache_lookup(key, &cached))
        return cached;

    /* recursion */
    seq_t total = 0;
    uint64_t max_v = rem / parts;               /* largest smallest part */
    for (uint64_t v = minv; v <= max_v; ++v) {
        uint64_t max_m = rem / v;
        if (max_m > parts)
            max_m = parts;
        for (uint64_t m = 1; m <= max_m; ++m) {
            uint64_t rem2 = rem - v * m;
            uint64_t rest = parts - m;
            /* after using value v m times, the next distinct value
               must be > v, hence minv = v+1 */
            seq_t sub = count_partitions(rem2, rest, distinct - 1, v + 1);
            total += sub;
        }
    }

    cache_insert(key, total);
    return total;
}

/* ---- public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                /* a(0) = 1 */

    /* --------------------------------------------------------------- */
    /* compute the minimal k such that n | k^2.  This is the product of
       p^{ceil(e/2)} for each prime p^e || n. */
    uint64_t N   = (uint64_t)n;
    uint64_t tmp = N;
    uint64_t k_min = 1;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            unsigned need = (exp + 1) / 2;   /* ceil(exp/2) */
            for (unsigned i = 0; i < need; ++i)
                k_min *= p;
        }
    }
    if (tmp > 1)                    /* remaining prime factor */
        k_min *= tmp;

    /* reset memoisation cache for this n */
    cache_generation++;

    /* --------------------------------------------------------------- */
    /* sum over all admissible k: k ranges over multiples of k_min
       up to n.  For each k compute d = k^2 / n and add the number
       of partitions of n into k parts with d distinct values. */
    seq_t result = 0;
    for (uint64_t k = k_min; k <= N; k += k_min) {
        /* k must satisfy n | k^2; with above step it always does,
           but we double‑check to be safe. */
        uint64_t sq = k * k;
        if (sq % N != 0)
            continue;
        uint64_t d = sq / N;        /* number of distinct values required */
        result += count_partitions(N, k, d, 1);
    }
    return result;
}
