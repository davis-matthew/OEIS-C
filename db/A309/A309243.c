/*
 * A309243  Completely multiplicative with a(p) = p * a(p-1) for any prime
 *          number p.
 * https://oeis.org/A309243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(1)=1. For a prime p, a(p)=p*a(p-1). The sequence is completely
 *            * multiplicative: a(mn)=a(m)*a(n). The implementation uses a
 *            memoised * recursive evaluation together with simple
 *            trial‑division prime tests that * are fast enough for the
 *            required range (n up to about 10000)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 282, u64 n <= 2878,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External integer types provided by the build. */
extern seq_t a(idx_t n);  // function required by the harness

/* ---------- Internal data ------------------------------------------ */

static seq_t *cache = NULL;   /* cache[n] holds a(n), 1‑based; 0 means not yet computed. */
static size_t cache_cap = 0;   /* current allocated size (number of slots). */

/* ------------------------------------------------------------- */
/* Ensure that the cache can address indices up to 'n'.  New entries are
 * initialised to 0.  cache[1] is set to 1 (the first value of the sequence). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();  /* allocation failure; abort is safe here */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_cap = new_cap;
    if (cache_cap > 1 && cache[1] == (seq_t)0) cache[1] = (seq_t)1;
}

/* ------------------------------------------------------------- */
/* Simple deterministic prime test suitable for n <= 10000
 * (trial division up to sqrt(n)).  This function is O(sqrt(n)) but
 * fast enough for the problem limits. */
static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ------------------------------------------------------------- */
/* Fast exponentiation for seq_t values.  The exponent is an idx_t
 * (64‑bit integer).  This works for all supported seq_t types. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1) result *= base;
        if (exp > 1) base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------- */
/* Recursive computation of a(n) with memoisation.  The caller
 * guarantees that the cache can address index n. */
static seq_t compute_a_rec(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* base case */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Prime case: a(p) = p * a(p-1).  Compute a(p-1) first. */
    if (is_prime_idx(n))
    {
        seq_t val = (seq_t)n * compute_a_rec(n - 1);
        cache[n] = val;
        return val;
    }

    /* Composite case: factor n and combine a(p)^e. */
    idx_t tmp = n;
    seq_t product = (seq_t)1;
    while (tmp > 1)
    {
        idx_t p;  /* smallest prime divisor of tmp */
        if (tmp % 2 == 0)
            p = 2;
        else
        {
            p = 0;
            for (idx_t d = 3; d * d <= tmp; d += 2)
                if (tmp % d == 0) { p = d; break; }
            if (p == 0) p = tmp;              /* tmp itself is prime */
        }
        idx_t exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }
        seq_t part = pow_seq(compute_a_rec(p), exp);
        product *= part;
    }

    cache[n] = product;
    return product;
}

/* ------------------------------------------------------------- */
/* Public function required by the harness.  Handles the
 * 1‑based index convention. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; safety */
    ensure_cache(n);
    return compute_a_rec(n);
}
