/*
 * A332675  Prime numbers p_k such that p_k == 3 (mod 10) and p_(k+1) == 1
 *          (mod 10).
 * https://oeis.org/A332675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332675: primes p_k with p_k ≡ 3 (mod 10) and p_(k+1) ≡ 1 (mod 10). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/next_prime.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_cnt = 0;

static uint64_t last_checked = 1;  /* start searching after this value          */
static uint64_t prev_prime = 0;     /* previous prime found                     */
static int prev_mod10 = 0;          /* previous prime mod 10 (0 means none)     */

/* Ensure the cache holds at least `need` entries. */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if ((size_t)need > new_cap)
        new_cap = (size_t)need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate sequence entries until at least `n` terms are cached. */
static void generate_up_to(idx_t n)
{
    while (cache_cnt < n)
    {
        uint64_t cand = next_prime(last_checked);  /* next prime > last_checked */
        int mod10 = (int)(cand % 10);

        /* If previous prime ≡ 3 (mod 10) and current prime ≡ 1 (mod 10),
           then previous prime is a term of the sequence. */
        if (prev_mod10 == 3 && mod10 == 1)
        {
            ensure_cache_cap(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)prev_prime;
        }

        /* Update state for the next iteration. */
        prev_prime = cand;
        prev_mod10 = mod10;
        last_checked = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for the OEIS offset 1, safe fallback */

    /* If we haven't computed enough terms yet, generate more. */
    if (cache_cnt < n)
        generate_up_to(n);

    return cache[n - 1];
}
