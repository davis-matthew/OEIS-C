/*
 * A367096  Irregular triangle read by rows where row n lists the semiprime
 *          divisors of n. Alternatively, row n lists the semiprime divisors
 *          of A002808(n).
 * https://oeis.org/A367096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = nth element of the flattened triangle of semiprime
 *            divisors of numbers, read row by row, skipping rows with no
 *            semiprime divisors
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   build_spf, cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a small‑prime‑factor (SPF) sieve for fast factorisation
   and memoises the sequence in a static cache. It avoids recursion and generates
   semiprime divisors directly from the prime factorisation of each row. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/build_spf.h"

/* ----------  cached sequence  ---------- */
static seq_t *seq_cache   = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;

/* ----------  SPF table  ---------- */
static idx_t *spf_ptr   = NULL;   /* spf_ptr[0] unused, spf_ptr[1] = 0 */
static idx_t spf_limit = 0;      /* current limit covered by spf_ptr */

/* ensure spf_ptr covers at least 'limit' */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;
    if (spf_ptr)
        free(spf_ptr);
    if (spf_limit == 0)
        spf_limit = 2;
    while (spf_limit < limit)
        spf_limit <<= 1;
    build_spf(spf_limit, &spf_ptr);
}

/* append one value to the cached sequence, grow if necessary */
static void seq_append(seq_t v)
{
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = (seq_cache_cap ? seq_cache_cap * 2 : 16);
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = v;
}

/* factorise n using the spf table; primes[], exps[] are pre‑allocated buffers
   (64 elements is sufficient for n <= 2^64).  *cnt receives number of distinct
   prime factors. */
static void factorize(idx_t n, idx_t primes[], int exps[], size_t *cnt)
{
    size_t c = 0;
    while (n > 1) {
        idx_t p = spf_ptr[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        primes[c] = p;
        exps[c]   = e;
        ++c;
    }
    *cnt = c;
}

/* process a single row: compute its semiprime divisors and append them in
   ascending order to the cache.  Rows with no semiprime divisors are ignored. */
static void process_row(idx_t row)
{
    if (row < 2)
        return;                         /* rows 1 and 2 have no semiprime divisors */

    ensure_spf(row);

    idx_t  primes[64];
    int    exps[64];
    size_t pf_cnt = 0;
    factorize(row, primes, exps, &pf_cnt);

    /* collect all semiprime divisors in a local array */
    seq_t divs[64];
    size_t divs_len = 0;

    /* products of two distinct primes that divide the row */
    for (size_t i = 0; i < pf_cnt; ++i) {
        /* p^2 if exponent >= 2 */
        if (exps[i] >= 2) {
            divs[divs_len++] = (seq_t)primes[i] * primes[i];
        }
        for (size_t j = i + 1; j < pf_cnt; ++j) {
            divs[divs_len++] = (seq_t)primes[i] * primes[j];
        }
    }

    if (divs_len == 0)
        return;                         /* no semiprime divisors in this row */

    /* sort the divisors */
    qsort(divs, divs_len, sizeof(seq_t), cmp_seq_t);

    /* append sorted divisors to the global cache */
    for (size_t i = 0; i < divs_len; ++i)
        seq_append(divs[i]);
}

/* next row that will be processed */
static idx_t next_row = 1;

/* sequence function returning the nth term of the flattened triangle */
seq_t a(idx_t n)
{
    /* build the cache up to the requested term */
    while (seq_cache_len < n) {
        process_row(next_row++);
    }
    return seq_cache[n - 1];
}
