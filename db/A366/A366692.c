/*
 * A366692  Primes that have more even digits than the next prime.
 * https://oeis.org/A366692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1826, u32 n <= 1154, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for primes that have more even decimal digits
   than the next consecutive prime.  It builds the sequence on demand
   and memoises the terms.  It is efficient enough for n up to about
   10 000 and works for both fixed‑size and arbitrary‑precision
   seq_t values. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_counts.h"

static unsigned even_digits(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    return (unsigned)(cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8]);
}

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

static seq_t cur_prime = (seq_t)2;  /* current prime being examined */

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

static seq_t next_prime(seq_t p)
{
    seq_t candidate = p + (seq_t)1;
    while (!is_prime_big(candidate))
        candidate = candidate + (seq_t)1;
    return candidate;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 0 */

    size_t want = (size_t)n;
    ensure_cache_cap(want);

    if (cache_len >= want)
        return cache[want - 1];

    while (cache_len < want) {
        seq_t next = next_prime(cur_prime);
        if (even_digits(cur_prime) > even_digits(next))
            cache[cache_len++] = cur_prime;
        cur_prime = next;
    }
    return cache[want - 1];
}
