/*
 * A329549  Numbers 4*k such that 1 is the last integer obtained when 4*k is
 *          successively divided by its divisors in increasing order.
 * https://oeis.org/A329549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A329549 by scanning multiples of 8 and testing the
 * divisor property: 1 must be the last integer obtained when the number
 * is successively divided by its divisors in increasing order.  This
 * implementation generates a fixed prime sieve up to 1 000 000 and
 * factors every candidate to derive its divisors.  Results are cached
 * so that arbitrary calls to a(idx_t) are answered in O(1) after a
 * single pass. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve – up to 1 000 000.  Sufficient for factoring the
   candidates required for n ≤ 10000.                                      */
#define PRIME_SIEVE_LIMIT 1000000U

static uint8_t  prime_sieve[PRIME_SIEVE_LIMIT + 1];
static uint32_t prime_list[PRIME_SIEVE_LIMIT];
static size_t   prime_count = 0;

static void init_prime_sieve(void)
{
    if (prime_count > 0)
        return;              /* already initialised */

    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i)
        prime_sieve[i] = 1;
    prime_sieve[0] = prime_sieve[1] = 0;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (prime_sieve[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                prime_sieve[j] = 0;

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (prime_sieve[i])
            prime_list[prime_count++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/* Helper for sorting divisors with qsort.  Works for any unsigned
   seq_t type. */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;

    if (*a == *b)
        return 0;
    return (*a < *b) ? -1 : 1;
}

/* Recursively build all divisors from the prime factorisation. */
static void gen_divisors_rec(size_t idx,
                             seq_t  cur,
                             const seq_t *pf,
                             const int  *exp,
                             size_t      pfcnt,
                             seq_t       *divs,
                             size_t     *dcnt)
{
    if (idx == pfcnt) {
        divs[(*dcnt)++] = cur;
        return;
    }

    seq_t val = 1;
    for (int i = 0; i <= exp[idx]; ++i) {
        gen_divisors_rec(idx + 1, cur * val, pf, exp,
                         pfcnt, divs, dcnt);
        val *= pf[idx];
    }
}

/* ------------------------------------------------------------------ */
/* Test whether a number satisfies the divisor‑division property. */
static bool passes(seq_t m)
{
    /* factorise m */
    init_prime_sieve();

    seq_t n   = m;
    seq_t pf[64];
    int   exp[64];
    size_t pfcnt = 0;

    for (size_t i = 0; i < prime_count &&
         (seq_t)prime_list[i] * (seq_t)prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pf[pfcnt]   = (seq_t)p;
            exp[pfcnt] = e;
            ++pfcnt;
        }
    }

    if (n > 1) {                     /* remaining prime factor */
        pf[pfcnt]   = n;
        exp[pfcnt] = 1;
        ++pfcnt;
    }

    /* generate all divisors */
    seq_t divisors[1024];
    size_t dcnt = 0;
    gen_divisors_rec(0, (seq_t)1, pf, exp, pfcnt,
                     divisors, &dcnt);

    /* sort divisors in ascending order */
    qsort(divisors, dcnt, sizeof(seq_t), cmp_seq_t);

    /* simulate the successive division process */
    seq_t x = m;
    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divisors[i];
        if (x % d != 0)
            break;
        x /= d;
        if (x == (seq_t)1)
            return true;              /* last integer seen is 1 */
    }
    return x == (seq_t)1;
}

/* ------------------------------------------------------------------ */
/* Cache for previously found terms.  The array grows as needed. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_push(seq_t value)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_buf;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Returns the n‑th term of A329549. */
seq_t a(idx_t n)
{
    static seq_t next_candidate = (seq_t)8;   /* multiples of 8 */

    if (n < 1)          /* offset starts at 1; behaviour for n<1 is undefined */
        return (seq_t)0;

    while (cache_len < (size_t)n) {
        if (passes(next_candidate))
            cache_push(next_candidate);
        next_candidate += (seq_t)8;          /* next multiple of 8 */
    }
    return cache[n - 1];
}
