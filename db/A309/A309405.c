/*
 * A309405  Number of prime parts in the partitions of n into 3 parts.
 * https://oeis.org/A309405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Sequence A309405: Number of prime parts in the partitions of n into 3 parts.
 *  Formula:
 *      a(n) = Σ_{j=1..⌊n/3⌋} Σ_{i=j..⌊(n-j)/2⌋}
 *             (c(i) + c(j) + c(n-i-j))
 *  where c(k) = 1 if k is prime, otherwise 0.
 *  This implementation uses a simple Sieve of Eratosthenes for prime
 *  testing, caching all results up to the largest n requested.
 */

#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------- */
/* Prime sieve state --------------------------------------------------- */
static bool * _is_prime   = NULL;   /* true if index is prime          */
static idx_t  _prime_lim  = 0;      /* number up to which primes are known */

/* Ensure that the sieve covers at least up to 'limit'                    */
static void _ensure_primes_up_to(idx_t limit)
{
    if (limit <= _prime_lim) return;          /* already sufficient  */

    bool *flag = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!flag) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= limit; ++i) flag[i] = true;
    flag[0] = flag[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (flag[p])
            for (idx_t m = p * p; m <= limit; m += p)
                flag[m] = false;

    free(_is_prime);
    _is_prime  = flag;
    _prime_lim = limit;
}

/* -------------------------------------------------------------------- */
/* Cache for computed a(n) values ------------------------------------- */
static seq_t * _cache    = NULL;   /* cache[0..max] contains a(n)   */
static idx_t  _cache_sz  = 0;      /* number of elements currently cached */

seq_t a(idx_t n)
{
    /* Prime table must include n (part values can be up to n). */
    _ensure_primes_up_to(n);

    /* Grow cache to include n if necessary */
    if (_cache_sz <= n)
    {
        size_t new_sz = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(_cache, new_sz * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);

        /* Compute a(i) for all new indices i = _cache_sz .. n   */
        for (idx_t i = _cache_sz; i <= n; ++i)
        {
            seq_t val = 0;
            if (i >= 3)      /* partitions into 3 parts exist only for i>=3 */
            {
                idx_t jmax = i / 3;     /* lowest part j */
                for (idx_t j = 1; j <= jmax; ++j)
                {
                    idx_t imax = (i - j) / 2;   /* middle part i */
                    for (idx_t m = j; m <= imax; ++m)
                    {
                        idx_t k = i - j - m;     /* largest part */
                        val += (seq_t)_is_prime[m];
                        val += (seq_t)_is_prime[j];
                        val += (seq_t)_is_prime[k];
                    }
                }
            }
            new_cache[i] = val;
        }

        _cache    = new_cache;
        _cache_sz = new_sz;
    }

    return _cache[n];
}
