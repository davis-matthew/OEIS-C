/*
 * A390307  Irregular triangle read by rows where row n lists the first sums
 *          of prime indices of n.
 * https://oeis.org/A390307
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   not stated
 * Range:     u8 n <= 6632, u16 n <= 7506, u32 n <= 7506, u64 n <= 7506,
 *            u128 n <= 7506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A390307:  a(n) is the n‑th term (offset 4) of the flattened
 * irregular triangle whose row n lists the first sums of the prime
 * indices of n.  The implementation builds prime indices with a
 * sieve and generates rows on demand.
 */

#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
#  define CAST(type, expr) reinterpret_cast<type>(expr)
#else
#  define CAST(type, expr) ((type)(expr))
#endif

/* global data */
static unsigned *spf = NULL;           // smallest prime factor
static uint32_t *prime_index = NULL;   // index of a prime, 0 if not prime
static idx_t sieve_limit = 0;          // largest index currently sieved

static seq_t *cache = NULL;            // flattened triangle terms
static size_t cache_size = 0;          // number of terms stored
static size_t cache_cap   = 0;          // capacity of cache array
static idx_t next_row = 4;             // next n to process

/* ---------------------------------------------------------------------- */
/* simple sieve that also records prime indices                         */
static void build_sieve(idx_t limit)
{
    if (spf) free(spf);
    if (prime_index) free(prime_index);

    spf = CAST(unsigned *, malloc((limit + 1) * sizeof(unsigned)));
    prime_index = CAST(uint32_t *, malloc((limit + 1) * sizeof(uint32_t)));
    if (!spf || !prime_index) return; /* out of memory */

    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (unsigned)i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (unsigned)i;
            }
        }
    }

    uint32_t idx = 0;
    for (idx_t i = 0; i <= limit; ++i) prime_index[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {
            ++idx;
            prime_index[(uint32_t)i] = idx;
        }
    }
    sieve_limit = limit;
}

/* ---------------------------------------------------------------------- */
/* ensure we have enough sieve space for the given row number            */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit) return;
    idx_t new_limit = sieve_limit ? sieve_limit * 2 : 256;
    while (new_limit < n) new_limit *= 2;
    build_sieve(new_limit);
}

/* ---------------------------------------------------------------------- */
/* process a single row (number n) and append its first sums to cache   */
static void process_row(idx_t n)
{
    idx_t temp = n;
    uint32_t items[64];   /* prime indices with multiplicity */
    int len = 0;

    while (temp > 1) {
        unsigned p = spf[temp];
        int count = 0;
        while (temp % p == 0) {
            temp /= p;
            ++count;
        }
        uint32_t pidx = prime_index[p];
        for (int i = 0; i < count; ++i) {
            if (len < 64) items[len++] = pidx;
        }
    }
    /* generate first sums */
    for (int i = 0; i + 1 < len; ++i) {
        seq_t val = (seq_t)items[i] + (seq_t)items[i + 1];
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *new_cache = CAST(seq_t *, realloc(cache, new_cap * sizeof(seq_t)));
            if (!new_cache) return; /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = val;
    }
}

/* ---------------------------------------------------------------------- */
/* public function required by the kernel                                */
seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined below offset */

    /* initialise sieve if necessary */
    if (!spf) build_sieve(256);

    /* generate rows until we have enough terms */
    while (cache_size < (size_t)(n - 3)) {
        ensure_sieve(next_row);
        process_row(next_row);
        ++next_row;
    }
    return cache[n - 4];
}
