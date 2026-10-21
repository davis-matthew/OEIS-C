/*
 * A271922  Denominator of n*Product_{j=1..n-1} ((3*j + 1)/(3*j + 2)).
 * https://oeis.org/A271922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of n * Prod_{j=1..n-1} (3*j+1)/(3*j+2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Prime sieve and factorisation helpers ------------------------------------- */
static int  *primes      = NULL;          /* list of primes up to current limit */
static int  *prime_index = NULL;          /* prime -> index in primes list, -1 if not prime */
static size_t  prime_count = 0;           /* number of primes */
static idx_t    primes_limit = 0;         /* current sieve limit */

/* Ensure primes up to 'limit' are available. Rebuild sieve if needed.      */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    /* Release old arrays */
    free(primes);
    free(prime_index);

    /* Allocate new arrays */
    primes = (int *)malloc((limit + 1) * sizeof(int));
    prime_index = (int *)malloc((limit + 1) * sizeof(int));
    /* Initialise indices to -1 (non-prime) */
    for (idx_t i = 0; i <= limit; ++i)
        prime_index[i] = -1;

    /* Simple sieve of Eratosthenes */
    char *is_comp = (char *)calloc(limit + 1, sizeof(char));
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;

    /* Collect primes and store indices */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) {
            primes[cnt]   = (int)i;
            prime_index[(int)i] = (int)cnt;
            cnt++;
        }

    free(is_comp);
    prime_count = cnt;
    primes_limit = limit;
}

/* Factorise 'num' and add exponents to 'exp_arr'                              */
static void factor_update(idx_t num, int *exp_arr)
{
    idx_t tmp = num;
    for (size_t i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp)
            break;
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                cnt++;
            }
            exp_arr[prime_index[p]] += cnt;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        exp_arr[prime_index[(int)tmp]] += 1;
    }
}

/* Cache of the sequence ---------------------------------------------- */
static seq_t  *den_cache   = NULL;   /* stored denominators */
static idx_t   cache_size  = 0;      /* number of cached terms */

static int  *exp_num = NULL;         /* exponents in numerator  */
static int  *exp_den = NULL;         /* exponents in denominator */
static size_t exp_count = 0;         /* length of exponent arrays */

/* Grow exponent arrays when new primes appear --------------------------- */
static void resize_exp_arrays(void)
{
    if (prime_count > exp_count) {
        exp_num = (int *)realloc(exp_num, prime_count * sizeof(int));
        exp_den = (int *)realloc(exp_den, prime_count * sizeof(int));
        for (size_t i = exp_count; i < prime_count; ++i) {
            exp_num[i] = 0;
            exp_den[i] = 0;
        }
        exp_count = prime_count;
    }
}

/* Ensure the cache contains all terms up to 'n' ------------------------ */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* Ensure we have primes up to the largest number we will factor */
    idx_t limit_needed = 3 * n + 2;
    ensure_primes(limit_needed);

    /* Initialise exponent arrays if needed */
    if (!exp_num) {
        exp_num = (int *)calloc(prime_count, sizeof(int));
        exp_den = (int *)calloc(prime_count, sizeof(int));
        exp_count = prime_count;
    } else
        resize_exp_arrays();

    /* Expand denominator cache */
    size_t new_size = (size_t)n;
    den_cache = (seq_t *)realloc(den_cache, new_size * sizeof(seq_t));

    /* Compute missing terms */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        if (k == 1) {          /* base case: 1 */
            den_cache[0] = (seq_t)1;
            continue;
        }

        /* Update exponents */
        factor_update(k,           exp_num);        /* numerator factor k          */
        factor_update(3 * (k - 1) + 1, exp_num);   /* numerator factor 3(k-1)+1   */
        factor_update(k - 1,       exp_den);        /* denominator factor k-1      */
        factor_update(3 * (k - 1) + 2, exp_den);   /* denominator factor 3(k-1)+2*/

        /* Compute denominator from exponent differences */
        seq_t denom = (seq_t)1;
        for (size_t i = 0; i < prime_count; ++i) {
            int diff = exp_den[i] - exp_num[i];
            if (diff > 0) {
                seq_t p = (seq_t)primes[i];
                for (int e = 0; e < diff; ++e)
                    denom *= p;
            }
        }
        den_cache[(size_t)k - 1] = denom;
    }

    cache_size = n;
}

/* Public sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1, return 0 for safety */

    ensure_cache(n);
    return den_cache[(size_t)n - 1];
}
