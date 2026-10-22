/*
 * A391352  Numbers k such that 3*k - (greatest prime < 3*k) = (least prime >
 *          3*k) - 3*k.
 * https://oeis.org/A391352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that 3*k - (greatest prime < 3*k) = * (least
 *            prime > 3*k) - 3*k, i.e. 6*k = p + q for consecutive primes p<q
 * Range:     u8 n <= 21, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below searches through successive primes and stores each k
 * where (p+q) is divisible by 6. It grows a dynamic cache on demand.
 * The implementation is O(n) and works for any supported seq_t and idx_t
 * type. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/* Dynamic array of primes (uint64_t).                                 */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;

static void grow_prime_cap(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t newcap = prime_cap ? prime_cap * 2 : 8;
    if (newcap < needed)
        newcap = needed;
    uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    if (!tmp && newcap != 0)
        abort(); /* allocation failure */
    primes = tmp;
    prime_cap = newcap;
}

/* Generate the next prime greater than the current largest prime.   */
static void generate_next_prime(void)
{
    idx_t candidate;
    if (prime_cnt == 0)
        candidate = 2;
    else
    {
        candidate = primes[prime_cnt - 1] + 1;
        /* Skip even numbers after 2 */
        if (candidate % 2 == 0)
            ++candidate;
    }

    while (!is_prime(candidate))
    {
        candidate += 2; /* test only odd candidates */
    }

    grow_prime_cap(prime_cnt + 1);
    primes[prime_cnt++] = (uint64_t)candidate;
}

/* ------------------------------------------------------------------ */
/* Cache of sequence values (seq_t).                                  */
/* ------------------------------------------------------------------ */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow the cache if needed when adding a new term. */
static void grow_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    if (newcap < needed)
        newcap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp && newcap != 0)
        abort(); /* allocation failure */
    cache = tmp;
    cache_cap = newcap;
}

/* Ensure the cache contains at least n terms.                        */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    while (cache_len < (size_t)n)
    {
        generate_next_prime();                         /* new prime added */
        if (prime_cnt >= 2)
        {
            seq_t sum = (seq_t)primes[prime_cnt - 2] + (seq_t)primes[prime_cnt - 1];
            if (sum % 6 == 0)                           /* divisible by 6 */
            {
                seq_t k = sum / 6;
                grow_cache_cap(cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function: returns the n‑th term of A391352.                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                    /* sequence is 1‑indexed; return 0 for n==0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
