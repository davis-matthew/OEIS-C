/*
 * A283166  a(0) = 0; a(1) = 1; a(2*n) = sigma(a(n)), a(2*n+1) = sigma(a(n)) +
 *          sigma(a(n+1)).
 * https://oeis.org/A283166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; a(2*n) = sigma(a(n)); a(2*n+1) = sigma(a(n))
 *            + sigma(a(n+1)); Sum of divisors sigma is defined in the OEIS
 *            tools
 * Range:     u8 n <= 136, u16 n <= 1348, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*---------------------------------------------*/
/* Memoisation cache for a(n) values            */
/*---------------------------------------------*/
static seq_t *cache = NULL;   /* static array holding computed values   */
static size_t cache_cap = 0;  /* current capacity of cache (entries 0..cache_cap-1) */

/* Ensure the cache can hold index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;            /* already large enough */

    size_t new_cap = (size_t)n + 1;                /* need room for indices 0..n */
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {                             /* allocation failed */
        /* abort – this should never happen in test harness */
        exit(1);
    }

    /* initialise the new part of the array to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Sum of divisors function sigma(x) for non‑negative integer x   */
static seq_t sigma(seq_t x)
{
    if (x <= 1) return x;           /* sigma(0)=0, sigma(1)=1 */

    seq_t prod = 1;                 /* product of prime‑power sums   */
    seq_t temp = x;                 /* remaining part to factorise  */

    for (idx_t i = 2; ; ++i) {
        seq_t ii = (seq_t)i * i;    /* i² */
        if (ii > temp) break;      /* no more factors possible     */

        if (temp % (seq_t)i == 0) {
            seq_t power = 1;       /* i⁰   */
            seq_t sum   = 1;       /* 1 + i + i² + …             */

            while (temp % (seq_t)i == 0) {
                temp /= (seq_t)i;
                power *= (seq_t)i;
                sum += power;      /* accumulate next power       */
            }
            prod *= sum;           /* combine with other factors  */
        }
    }

    /* if temp > 1, it is a remaining prime factor */
    if (temp > 1)
        prod *= (temp + 1);        /* (p^1 + 1) */

    return prod;
}

/*-----------------------------------------------------*/
/* Public sequence function a(idx_t n)                 */
/*-----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];   /* already computed */

    idx_t k = n >> 1;                     /* floor(n/2) */

    if ((n & 1) == 0) {                   /* even index: a(2*k) */
        seq_t val = a(k);
        cache[n] = sigma(val);
    } else {                              /* odd index: a(2*k+1) */
        seq_t val1 = a(k);
        seq_t val2 = a(k + 1);
        cache[n] = sigma(val1) + sigma(val2);
    }

    return cache[n];
}
