/*
 * A350230  Numbers m such that for all factorizations m=a*b with positive
 *          integers (a, b), a*b+a+b is a prime.
 * https://oeis.org/A350230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) computed by checking all integers m: m is kept if for *
 *            every factorisation m = a * b, the number a*b + a + b is prime.
 *            * No simple closed‑form formula is known; this implementation *
 *            lazily generates the sequence and caches the results
 * Range:     u8 n <= 26, u16 n <= 1283, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/factor_u64.h"

/* Recursive enumeration of divisors formed from the distinct prime
 * factors stored in `primes[0..cnt-1]`.  For each divisor a (other
 * than 1 and m) the function checks whether a*b + a + b is prime. */
static bool dfs_divs(uint64_t cur, size_t pos, uint64_t m,
                     uint64_t primes[], size_t cnt)
{
    for (size_t i = pos; i < cnt; ++i) {
        uint64_t a = cur * primes[i];
        if (a > m) continue;                 /* overflow guard */

        if (a != 1 && a != m) {
            seq_t val = (seq_t)a * (seq_t)(m / a)
                        + (seq_t)a + (seq_t)(m / a);
            if (!isPrimeGeneric(val))
                return false;
        }

        if (!dfs_divs(a, i + 1, m, primes, cnt))
            return false;
    }
    return true;
}

/* Test whether `m` satisfies the sequence definition. */
static bool is_valid(uint64_t m)
{
    if (m == 1)                    /* 1 is always accepted */
        return true;

    /* Factorisation 1 * m: needs 2*m + 1 to be prime. */
    if (!isPrimeGeneric((seq_t)((seq_t)2 * m + 1)))
        return false;

    /* A prime m automatically satisfies all other factorizations. */
    if (isPrimeGeneric((seq_t)m))
        return true;

    /* Factor m and check square‑freeness. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > 1)
            return false;           /* not square‑free */

    /* Enumerate proper divisors and confirm the primality test. */
    return dfs_divs(1, 0, m, primes, cnt);
}

/* --------------------- caching infrastructure --------------------- */

static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint64_t next_candidate = 0;   /* next integer to test */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_cap(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Append a value to the cache, enlarging it if necessary. */
static void cache_append(seq_t val)
{
    ensure_cache_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* --------------------- public interface --------------------- */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; value for n==0 is undefined */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++next_candidate;
        if (is_valid(next_candidate))
            cache_append((seq_t)next_candidate);
    }
    return seq_cache[n - 1];
}
