/*
 * A249761  Primes p such that there is prime q with sigma(q-1) = p.
 * https://oeis.org/A249761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249761: primes p such that there exists a prime q with σ(q‑1)=p.
   Algorithm: iterate increasing primes q, compute σ(q‑1) using sigma_of.
   If σ(q‑1) is prime, record the resulting prime p.
   Results are cached, sorted and deduplicated so that a(n) returns the
   nth term (1‑based).  If n exceeds the number of terms found, 0 is
   returned.                                    */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static seq_t *seq_cache = NULL;   /* sorted unique list of found primes p   */
static size_t cache_len = 0;      /* current number of stored terms          */
static size_t cache_cap = 0;      /* allocated capacity of seq_cache         */

static uint64_t *prime_list = NULL;   /* list of primes q                       */
static size_t prime_cnt = 0;          /* number of primes in prime_list          */
static idx_t   prime_limit = 0;       /* current sieve limit                     */
static idx_t   prime_index = 0;       /* next prime to process                   */

/* Ensure seq_cache contains at least n terms (1‑based).  Extends the
   list of primes and processes more q values as needed.  Returns
   when cache_len >= n.                                         */
static void ensure_cache(idx_t n)
{
    /* Grow prime list if necessary.  The list is re‑sieved at higher limits. */
    while (cache_len < (size_t)n) {
        /* If all primes at the current limit have been processed, extend.   */
        if (prime_index >= prime_cnt) {
            if (prime_limit == 0) prime_limit = 1000;
            else prime_limit <<= 1;           /* double the limit            */

            uint64_t *new_list;
            size_t new_cnt;
            build_primes_upto(prime_limit, &new_list, &new_cnt);

            free(prime_list);                 /* free old allocation        */
            prime_list = new_list;
            prime_cnt = new_cnt;
            /* prime_index remains valid – it points to the next prime
               in the new enlarged list.                                  */
        }

        /* Process the next prime q.                                   */
        idx_t q = prime_list[prime_index++];
        seq_t p = sigma_of(q - 1);                 /* p = σ(q-1)                 */

        /* If p is prime, record it.                                  */
        if (is_prime_big(p))
            append_candidate(&seq_cache, &cache_len, &cache_cap, p);
    }

    /* Sort cache and remove duplicates.  This keeps seq_cache in
       sorted unique order for all future calls.                     */
    if (cache_len > 0) {
        mem_qsort(seq_cache, cache_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < cache_len; ++i) {
            if (uniq == 0 || seq_cache[i] != seq_cache[uniq - 1])
                seq_cache[uniq++] = seq_cache[i];
        }
        cache_len = uniq;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – n==0 is undefined    */
    ensure_cache(n);
    if ((size_t)n <= cache_len)
        return seq_cache[n - 1];
    return 0;                     /* sequence exhausted (finite case)     */
}
