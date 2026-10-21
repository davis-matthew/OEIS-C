/*
 * A276420  Number of palindromic compositions of n into prime parts.
 * https://oeis.org/A276420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 117, u64 n <= 231,
 *            u128 n <= 459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count palindromic compositions of n into prime parts.
   a(0)=1.
   Let C[s] be the number of (ordered) compositions of s into primes,
   with C[0]=1.  Then for any n≥0
        a(n) = (n even ? C[n/2] : 0)
               + Σ_{p prime, p≤n, (n-p) even}  C[(n-p)/2]            */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --- global (static) data ----------------------------------------- */
static uint64_t *primes_arr   = NULL;   /* list of primes up to primes_max */
static size_t   primes_cnt    = 0;      /* number of stored primes */
static idx_t    primes_max    = 0;      /* largest index such that primes_arr[j] ≤ primes_max */

static seq_t   *c_arr         = NULL;   /* C[0..c_max] */
static idx_t    c_max         = 0;      /* largest index such that c_arr[j] is valid */

/* --- helper: build primes up to limit -------------------------------- */
static void recompute_primes(idx_t limit)
{
    size_t sz = (size_t)limit + 1;
    bool *isprime = (bool*)malloc(sz);
    if (!isprime) abort();
    memset(isprime, 1, sz);
    isprime[0] = isprime[1] = 0;
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i)
        if (isprime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                isprime[j] = 0;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isprime[i]) ++cnt;

    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isprime[i]) new_primes[idx++] = i;

    free(primes_arr);
    free(isprime);
    primes_arr = new_primes;
    primes_cnt = cnt;
    primes_max = limit;
}

/* Ensure all primes ≤ limit are stored.                               */
static void ensure_primes(idx_t limit)
{
    if (primes_max >= limit)
        return;
    recompute_primes(limit);
}

/* --- helper: build C[0..limit] ------------------------------------- */
static void ensure_c(idx_t limit)
{
    if (c_max >= limit)
        return;

    size_t new_sz = (size_t)limit + 1;
    seq_t *new_arr = (seq_t*)realloc(c_arr, new_sz * sizeof(seq_t));
    if (!new_arr) abort();
    c_arr = new_arr;
    /* initialise new entries to 0 */
    for (idx_t i = c_max + 1; i <= limit; ++i)
        c_arr[i] = (seq_t)0;
    /* set base */
    if (c_max == 0)
        c_arr[0] = (seq_t)1;

    /* compute new entries using primes already available */
    for (idx_t x = c_max + 1; x <= limit; ++x) {
        seq_t sum = 0;
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            uint64_t p = primes_arr[pi];
            if (p > x) break;
            sum += c_arr[x - p];
        }
        c_arr[x] = sum;
    }
    c_max = limit;
}

/* ------------------------------------------------------------------- */
/* The required public function                                     */
seq_t a(idx_t n)
{
    /* Handle the trivial small cases first */
    if (n == 0) return (seq_t)1;

    /* Prime data must cover all possible middle parts */
    ensure_primes(n);

    /* We need composition counts up to floor(n/2)          */
    idx_t half_limit = n >> 1;          /* floor(n/2)  */
    ensure_c(half_limit);

    seq_t result = 0;

    /* Even‑length palindromes: half sum equals n/2 */
    if ((n & 1) == 0)
        result += c_arr[half_limit];

    /* Odd‑length palindromes: choose a prime middle part p */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint64_t p = primes_arr[pi];
        if (p > n) break;
        idx_t rem = n - p;
        if ((rem & 1) == 0)            /* total must be even after removing middle */
            result += c_arr[rem >> 1];
    }

    return result;
}
