/*
 * A362420  Partial sum of the first n odd semiprimes.
 * https://oeis.org/A362420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} A046315(i) (partial sum of the first n odd
 *            semiprimes)
 * Range:     u8 n <= 8, u16 n <= 162, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* The build system supplies the following types:
   - idx_t  : unsigned index type, typically uint64_t
   - seq_t  : unsigned integer type, may be fixed-width or an arbitrary‑precision class
 */

/* --- Static state for memoization ------------------------------------- */
static uint32_t *spf          = NULL;  /* smallest prime factor array   */
static idx_t   current_limit  = 0;     /* current sieve upper bound     */
static idx_t   last_checked   = 1;     /* last odd number examined      */
static size_t  semip_cnt      = 0;     /* number of odd semiprimes found*/
static size_t  pref_cap       = 0;     /* capacity of prefix sum array  */
static seq_t  *pref           = NULL;  /* pref[i] = sum of first i+1 odd semiprimes */

/* Build or rebuild a sieve of small prime factors up to new_limit */
static void rebuild_sieve(idx_t new_limit)
{
    free(spf);
    spf = (uint32_t *)calloc((size_t)new_limit + 1U, sizeof(uint32_t));
    if (!spf) abort();

    for (idx_t i = 0; i <= new_limit; ++i) spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * (idx_t)i <= new_limit; ++i)
        if (spf[i] == (uint32_t)i)   /* i is prime */
        {
            for (idx_t j = i * (idx_t)i; j <= new_limit; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    current_limit = new_limit;
}

/* Test whether x is an odd semiprime */
static inline bool is_odd_semiprime(idx_t x)
{
    if ((x & 1U) == 0) return false;  /* even */
    if (x < 9) return false;          /* smallest odd semiprime is 9 */

    int cnt = 0;
    idx_t tmp = x;
    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        if (p == 2) return false;     /* contains factor 2 */
        ++cnt;
        if (cnt > 2) return false;    /* more than two prime factors */
        tmp /= p;
    }
    return cnt == 2;
}

/* Append a new odd semiprime to the prefix sum array */
static void add_odd_semiprime(idx_t val)
{
    if (semip_cnt == pref_cap)
    {
        pref_cap = pref_cap ? pref_cap * 2 : 16;
        pref = (seq_t *)realloc(pref, pref_cap * sizeof(*pref));
        if (!pref) abort();
    }
    seq_t new_sum = (semip_cnt > 0 ? pref[semip_cnt - 1] : (seq_t)0);
    new_sum += (seq_t)val;
    pref[semip_cnt] = new_sum;
    ++semip_cnt;
}

/* Ensure that the first n odd semiprimes are computed */
static void ensure_n_terms(idx_t n)
{
    if (n <= semip_cnt) return;

    /* Grow the sieve range until we accumulate at least n terms */
    while (semip_cnt < n)
    {
        if (current_limit == 0) current_limit = 1000;
        else                   current_limit <<= 1;   /* double the limit */

        rebuild_sieve(current_limit);

        /* Scan odd numbers from the last processed one up to the new limit */
        for (idx_t num = last_checked + 2U; num <= current_limit; num += 2U)
        {
            if (is_odd_semiprime(num))
                add_odd_semiprime(num);
            last_checked = num;
        }
    }
}

/* --- Public function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, guard anyway */
    ensure_n_terms(n);
    return pref[n - 1];
}
