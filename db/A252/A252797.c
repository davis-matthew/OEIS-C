/*
 * A252797  Largest of n consecutive integers whose product is divisible by at
 *          least n distinct prime factors.
 * https://oeis.org/A252797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A252797: Largest of n consecutive integers whose product is
 * divisible by at least n distinct prime factors.
 *
 * This implementation searches, for a given n, the smallest window of
 * n consecutive integers that contains at least n distinct prime
 * factors in the product.  It keeps track of the prime factors using
 * a sieve that supplies the smallest prime factor of each integer
 * (spf array).  The result is returned as type seq_t.
 *
 * The function is memoised per n and may be called in arbitrary order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Global tables for sieve and window bookkeeping. */
static uint32_t *spf        = NULL;      /* smallest prime factor */
static int      *prime_cnt  = NULL;      /* count of a prime in window */
static uint32_t *prime_mark = NULL;      /* last tick a prime was seen */
static uint32_t tick        = 1;          /* per-call counter */
static uint32_t spf_limit   = 0;          /* current sieve bound */

static seq_t   cache[20001];              /* memoised a(n) values (max n=20000) */
static bool    cache_seen[20001] = { false }; /* seen flags */

/* Extend spf array to at least new_limit (inclusive) and perform sieve. */
static void extend_limit(uint32_t new_limit)
{
    if (new_limit <= spf_limit) return;

    uint32_t old_limit = spf_limit;
    uint32_t sz        = new_limit + 1;

    spf = (uint32_t*)realloc(spf, sz * sizeof(uint32_t));
    prime_cnt = (int*)realloc(prime_cnt, sz * sizeof(int));
    prime_mark = (uint32_t*)realloc(prime_mark, sz * sizeof(uint32_t));

    for (uint32_t i = old_limit + 1; i <= new_limit; ++i) {
        spf[i]        = 0;
        prime_cnt[i]  = 0;
        prime_mark[i] = 0;
    }

    /* classic linear sieve for spf */
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= new_limit) {
                for (uint32_t j = i * i; j <= new_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = new_limit;
}

/* Adjust the window bookkeeping for number x.  If add==true, x enters
 * the window; otherwise it leaves.  *distinct counts how many
 * distinct primes are currently present. */
static void adjust(uint32_t x, bool add, int *distinct)
{
    if (x <= 1) return;
    while (x > 1) {
        uint32_t p = spf[x];
        /* spf is guaranteed to be set because we extend spf before factoring. */
        if (prime_mark[p] != tick) {
            prime_mark[p] = tick;
            prime_cnt[p] = 0;
        }
        if (add) {
            if (prime_cnt[p] == 0) ++(*distinct);
            ++prime_cnt[p];
        } else {
            --prime_cnt[p];
            if (prime_cnt[p] == 0) --(*distinct);
        }
        uint32_t div;
        while ((div = x / p) * p == x) x = div;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but keep safe */
    if (n < (idx_t)sizeof(cache_seen)/sizeof(cache_seen[0]) && cache_seen[n])
        return cache[n];

    /* initialise sieve tables once */
    if (spf == NULL) {
        uint32_t init = 200000;          /* arbitrary start */
        extend_limit(init);
        tick = 1;
    }

    ++tick;
    if (tick == 0) {                    /* wrap‑around safety */
        tick = 1;
        for (uint32_t i = 0; i <= spf_limit; ++i) {
            prime_mark[i] = 0;
            prime_cnt[i]  = 0;
        }
    }

    uint32_t start = 1;
    uint32_t end   = 0;
    int distinct  = 0;

    while (true) {
        ++end;
        if (end > spf_limit) extend_limit(end);
        adjust(end, true, &distinct);         /* add new rightmost number */

        while (end - start + 1 > n) {          /* shrink from left */
            adjust(start, false, &distinct);
            ++start;
        }

        if (end - start + 1 == n && distinct >= (int)n) {
            seq_t result = (seq_t)end;
            if (n < (idx_t)sizeof(cache_seen)/sizeof(cache_seen[0])) {
                cache[n] = result;
                cache_seen[n] = true;
            }
            return result;
        }
    }
}
