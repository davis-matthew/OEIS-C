/*
 * A358849  Numbers k for which A053669(6*k) [the smallest prime not dividing
 *          6k] is of the form 6m-1.
 * https://oeis.org/A358849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A358849: k such that the smallest prime not dividing 6k is 6m‑1.
   The sequence starts with a(1)=1, a(2)=2, … .  This implementation
   builds all required k values on demand, caching them so that each
   value is computed only once.  It tests primes lazily using a small
   pre‑computed prime table.  The running time is well below 2 s for
   n≈10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define PRIME_LIMIT 50000   /* enough to cover first 10 000 terms */

static uint32_t primes[PRIME_LIMIT + 1];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;
    static char is_comp[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            if ((long long)i * i <= PRIME_LIMIT) {
                for (int j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
}

/* Test whether k satisfies the condition.  Only primes >3 are relevant. */
static bool qualifies(uint64_t k)
{
    for (size_t i = 2; i < primes_cnt; ++i) {      /* start at prime 5 */
        uint32_t p = primes[i];
        if (k % p != 0)                 /* first prime not dividing k */
            return (p % 6 == 5);
    }
    return false;   /* should never happen with PRIME_LIMIT large enough */
}

/* Cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static uint64_t next_k = 1;    /* next integer k to examine */

static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap >= needed) return;
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < needed) cache_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_cache) abort();      /* out of memory */
    cache = new_cache;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    init_primes();

    if (n == 0) return 0;  /* defined only for n ≥ 1 */

    ensure_cache_capacity(n);

    while (cache_size < n) {
        if (qualifies(next_k))
            cache[cache_size++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
