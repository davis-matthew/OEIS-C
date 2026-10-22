/*
 * A303741  Numbers k such that A(k+1) = A(k) + 2, where A() = A005101() are
 *          the abundant numbers.
 * https://oeis.org/A303741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given n, a(n) = the index k for which the (k)-th and (k+1)-th
   abundant numbers differ by 2.  The sequence starts 2,7,10,14,16,19,22,...
   The function is memoised; any number of calls in any order is O(1) after
   the first several evaluations. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static idx_t *spf = NULL;          /* smallest prime factor for every n */
static size_t spf_size = 0;        /* allocated size of spf array            */

static idx_t *diff_idx = NULL;     /* indices k where difference == 2      */
static size_t diff_len = 0;        /* number of values found so far          */
static size_t diff_cap = 0;        /* capacity of diff_idx array             */

static idx_t abundant_cnt = 0;     /* number of abundant numbers seen       */
static idx_t prev_abundant = 0;    /* value of previous abundant number    */
static idx_t current_n = 1;        /* next integer to test                  */

/* Simple build of smallest prime factor table up to `limit`. */
static void build_spf(idx_t limit)
{
    if (spf != NULL && spf_size > limit)
        return;                     /* already sufficient */

    idx_t new_size = limit + 1;
    idx_t *new_spf = (idx_t*)realloc(spf, new_size * sizeof(idx_t));
    if (!new_spf)
        abort();
    spf = new_spf;

    for (idx_t i = 2; i < new_size; ++i)
        spf[i] = i;                  /* initialise to itself */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] != i)                /* not prime */
            continue;
        for (idx_t j = i * i; j <= limit; j += i)
            if (spf[j] == j)
                spf[j] = i;
    }
    spf_size = new_size;
}

/* Test whether n is an abundant number using the spf table.
   The sum of divisors is computed in 128‑bit to avoid overflow. */
static int is_abundant(idx_t n)
{
    if (n < 2)
        return 0;

    __int128 sum = 1;          /* multiplicative identity for σ(n)   */
    idx_t m = n;
    while (m > 1) {
        idx_t p = spf[m];
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        __int128 term = 1;      /* sum of powers of p : 1 + p + ... + p^exp */
        __int128 powp = 1;
        for (unsigned int i = 1; i <= exp; ++i) {
            powp *= p;
            term += powp;
        }
        sum *= term;
        if (sum > (__int128)2 * n)
            return 1;          /* early exit: already abundant */
    }
    return sum > (__int128)2 * n;
}

/* Grow the buffer that stores the indices of the desired sequence. */
static void grow_diff_buf(size_t needed)
{
    if (diff_cap >= needed)
        return;
    size_t new_cap = diff_cap ? diff_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    idx_t *tmp = (idx_t*)realloc(diff_idx, new_cap * sizeof(idx_t));
    if (!tmp)
        abort();
    diff_idx = tmp;
    diff_cap = new_cap;
}

/* Ensure that at least `need` terms of the sequence are known. */
static void ensure_computed(idx_t need)
{
    if (diff_len >= (size_t)need)
        return;

    /* Determine a safe upper bound for spf – grow in chunks for speed. */
    const idx_t chunk = 200000;
    while (diff_len < (size_t)need) {
        idx_t limit = current_n + chunk - 1;
        build_spf(limit);                        /* ensures spf up to limit */

        while (current_n <= limit && diff_len < (size_t)need) {
            if (is_abundant(current_n)) {
                if (abundant_cnt > 0) {
                    if (current_n - prev_abundant == 2) {
                        grow_diff_buf(diff_len + 1);
                        diff_idx[diff_len++] = abundant_cnt;   /* k = index of previous abundant */
                    }
                }
                prev_abundant = current_n;
                ++abundant_cnt;
            }
            ++current_n;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n=0, but safe return */

    ensure_computed(n);
    return (seq_t)diff_idx[n - 1];
}
