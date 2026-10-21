/*
 * A294336  Number of ways to write n as a finite power-tower a^(b^(c^...)) of
 *          positive integers greater than one.
 * https://oeis.org/A294336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n)=∑_{d|A052409(n)} a(d) * where A052409(n)
 *            is the largest k such that n=m^k with m>1
 * Range:     u8 n <= 9380, u16 n <= 9380, u32 n <= 9380, u64 n <= 9380,
 *            u128 n <= 9380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static state for memoisation */
static seq_t *a_cache   = NULL;  /* sequence values a[0..a_cache_sz] */
static idx_t  a_cache_sz = 0;    /* last index computed (inclusive) */

static uint64_t *spf      = NULL;   /* smallest prime factor array */
static idx_t     spf_sz   = 0;      /* current size of spf array */

/* forward declaration of internal helper */
static void ensure_cache(idx_t n);

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for 0, safe guard */
    ensure_cache(n);
    return a_cache[n];
}

/* ------------------------------------------------------------ */
/* Ensure that a_cache[0..n] and spf[0..n] exist and a_cache[n] is computed. */
static void
ensure_cache(idx_t n)
{
    if (n <= a_cache_sz)
        return;                     /* already available */

    /* grow the a_cache array */
    seq_t *new_a = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!new_a)
        return;                     /* allocation failure – let caller handle */
    if (a_cache) {
        memcpy(new_a, a_cache, (a_cache_sz + 1) * sizeof(seq_t));
        free(a_cache);
    }
    a_cache = new_a;

    /* grow the spf array */
    uint64_t *new_spf = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!new_spf)
        return;                     /* allocation failure */
    if (spf) {
        free(spf);
    }
    spf = new_spf;

    /* initialise spf array for all indices up to n */
    for (idx_t i = 0; i <= n; ++i)
        spf[i] = i;

    /* sieve of Eratosthenes for smallest prime factor */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;
        }
    }

    /* make sure the base case a(1)=1 is defined */
    if (n >= 1)
        a_cache[1] = (seq_t)1;

    /* compute new values from previous size+1 up to n */
    idx_t old_size = a_cache_sz;
    a_cache_sz = n;                 /* extend the logical size first */

    /* start computing from max(old_size+1,2) to avoid recomputing a[1] */
    idx_t start = old_size + 1;
    if (start <= 1)
        start = 2;

    for (idx_t i = start; i <= n; ++i) {
        /* Compute A052409(i): gcd of the exponents in the prime factorisation */
        idx_t e = 0;
        idx_t tmp = i;
        while (tmp > 1) {
            idx_t p = spf[tmp];
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            if (e == 0)
                e = cnt;
            else {
                /* Euclidean GCD of e and cnt */
                idx_t a = e, b = cnt;
                while (b) {
                    idx_t t = a % b;
                    a = b;
                    b = t;
                }
                e = a;
            }
        }

        /* Sum a(d) for all divisors d of e (e ≥ 1) */
        seq_t sum = 0;
        for (idx_t d = 1; d <= e; ++d) {
            if (e % d == 0)
                sum += a_cache[d];
        }
        a_cache[i] = sum;
    }
}
