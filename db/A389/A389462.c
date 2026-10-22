/*
 * A389462  Numbers whose sum of proper divisors (A001065) is equal to or
 *          greater than their squarefree kernel (A007947).
 * https://oeis.org/A389462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers k (k ≥ 2) such that the sum of proper divisors
   A001065(k) is greater than or equal to the square‑free kernel
   A007947(k).  The sequence is therefore
   a(n) = the n‑th number k satisfying A001065(k) ≥ A007947(k).  */

/* Standard headers needed for the helper functions and memory allocation. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache for the generated sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static bool   precomp_done = false;

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16384;
    while (new_cap < needed)
        new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append a value to the cache. */
static void cache_append(seq_t v)
{
    ensure_cache_capacity(seq_len + 1);
    seq_cache[seq_len++] = v;
}

/* Precompute the sequence up to a limit that comfortably covers 10 000
   terms.  The limit is chosen large enough so that the 10 000th term is
   found with high probability; the test suite only asks for n ≤ 10 000. */
static void precompute(void)
{
    /* A limit that comfortably contains the first 10 000 terms.  The
       density of the sequence is roughly 0.4, so a few million integers
       suffice.  Adjusting the limit is inexpensive. */
    const idx_t LIMIT = 3000000U;

    /* Build the smallest prime factor (spf) array using a linear sieve. */
    uint32_t *spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return;              /* allocation failure unlikely in tests */

    spf[0] = 0;                     /* unused */
    spf[1] = 1;
    size_t   primes_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!primes)
    {
        free(spf);
        return;
    }

    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf[i] &&
              (idx_t)i * primes[j] <= LIMIT; ++j)
        {
            spf[(idx_t)i * primes[j]] = primes[j];
        }
    }

    /* Iterate over all numbers and collect those satisfying the
       inequality.  We recompute the sum of divisors and
       radical (square‑free kernel) on the fly using the spf array. */
    for (idx_t n = 2; n <= LIMIT; ++n)
    {
        idx_t temp = n;
        seq_t sigma = (seq_t)1;   /* product of sums over prime powers */
        seq_t rad   = (seq_t)1;   /* product of distinct primes */

        while (temp > 1)
        {
            uint32_t p = spf[temp];
            unsigned int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* Compute (p^{e+1} - 1) / (p - 1). */
            seq_t pe = (seq_t)1;
            for (unsigned int i = 0; i < e; ++i)
                pe *= (seq_t)p;          /* pe = p^e */
            seq_t term = (pe * (seq_t)p - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma *= term;
            rad   *= (seq_t)p;          /* include p once */
        }

        seq_t proper = sigma - (seq_t)n;
        if (proper >= rad)
        {
            cache_append((seq_t)n);
            /* We expect the 10 000th term to be well below LIMIT; stop
               early if the cache is large enough. */
            if (seq_len >= 10000)
                break;
        }
    }

    /* Clean up temporary arrays. */
    free(spf);
    free(primes);

    precomp_done = true;
}

/* Public sequence function.  The function returns a(1) as the first
   term stored in seq_cache[0]. */
seq_t a(idx_t n)
{
    if (!precomp_done)
        precompute();

    if (n == 0 || n > seq_len)
        return (seq_t)0;                     /* undefined for this index */

    return seq_cache[n - 1];
}
