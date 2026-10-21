/*
 * A256285  a(n) = smallest divisor of the concatenation of n+1 and n, that
 *          did not occur earlier.
 * https://oeis.org/A256285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A256285: a(n) is the smallest divisor of the concatenation of (n+1) and n
 *  that has not appeared earlier.  The implementation uses lazy evaluation,
 *  caching values up to the highest requested index and keeping a list of
 *  previously used divisors.  It works for any configured seq_t and idx_t.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_count.h"    /* digit_count(uint64_t) -> digit count */

/* ---------- Globals for caching ---------- */
static seq_t *seq_cache = NULL;   /* a_cache[1] == a(1) */
static size_t seq_cache_size = 0; /* number of computed terms (sequence index completes) */
static size_t seq_cache_cap  = 0;

static seq_t *used_cache = NULL;  /* list of all a(k) so far */
static size_t used_size   = 0;
static size_t used_cap    = 0;

/* ---------- Prime table ---------- */
#define PRIME_TABLE_SIZE 200000
static uint32_t primes[PRIME_TABLE_SIZE];
static size_t prime_count = 0;
static bool primes_ready = false;

/* ---------- Helper functions ---------- */
static void grow_seq_cache(size_t needed_cap)
{
    if (needed_cap <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
    while (new_cap < needed_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

static void grow_used_cache(size_t needed_cap)
{
    if (needed_cap <= used_cap) return;
    size_t new_cap = used_cap ? used_cap << 1 : 16;
    while (new_cap < needed_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(used_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    used_cache = tmp;
    used_cap = new_cap;
}

static bool used_contains(seq_t v)
{
    for (size_t i = 0; i < used_size; ++i)
        if (used_cache[i] == v) return true;
    return false;
}

static void append_used(seq_t v)
{
    grow_used_cache(used_size + 1);
    used_cache[used_size++] = v;
}

/* ---------- Prime table generation ---------- */
static void init_primes(void)
{
    if (primes_ready) return;
    bool *sieve = (bool *)malloc(PRIME_TABLE_SIZE + 1);
    if (!sieve) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= PRIME_TABLE_SIZE; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t p = 2; p * p <= PRIME_TABLE_SIZE; ++p) {
        if (sieve[p]) {
            for (size_t k = p * p; k <= PRIME_TABLE_SIZE; k += p)
                sieve[k] = false;
        }
    }
    prime_count = 0;
    for (size_t p = 2; p <= PRIME_TABLE_SIZE; ++p)
        if (sieve[p]) primes[prime_count++] = (uint32_t)p;
    free(sieve);
    primes_ready = true;
}

/* ---------- Factorization and divisor generation ---------- */
typedef struct { seq_t p; int exp; } factor_t;

/* generate all divisors of number with factorization in fac[0..fcnt-1] */
static void gen_divisors_rec(size_t idx, seq_t cur, seq_t *out, size_t *cnt,
                             const factor_t *fac, size_t fcnt)
{
    if (idx == fcnt) {
        out[(*cnt)++] = cur;
        return;
    }
    seq_t val = cur;
    for (int e = 0; e <= fac[idx].exp; ++e) {
        gen_divisors_rec(idx + 1, val, out, cnt, fac, fcnt);
        val = val * fac[idx].p;
    }
}

/* compute a single term at index k (1‑based) */
static seq_t compute_term(idx_t k)
{
    /* concatenate (k+1) and k */
    seq_t nk   = (seq_t)k;
    seq_t nk1  = nk + 1;
    unsigned d = (unsigned)digit_count((uint64_t)k);
    seq_t pow10 = 1;
    for (unsigned i = 0; i < d; ++i) pow10 *= 10;
    seq_t x = nk1 * pow10 + nk;

    /* factorization */
    factor_t fac[32];
    size_t fcnt = 0;
    seq_t rem = x;
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            fac[fcnt].p = p;
            fac[fcnt].exp = e;
            ++fcnt;
        }
    }
    if (rem > 1) {
        fac[fcnt].p = rem;
        fac[fcnt].exp = 1;
        ++fcnt;
    }

    /* collect divisors */
    static seq_t divs[16384];
    size_t divcnt = 0;
    gen_divisors_rec(0, 1, divs, &divcnt, fac, fcnt);

    /* find smallest unused divisor */
    seq_t best = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        if (!used_contains(divs[i])) {
            if (best == 0 || divs[i] < best) best = divs[i];
        }
    }
    return best;
}

/* ---------- Main public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1 */
    init_primes();

    /* compute all missing terms up to n */
    for (idx_t k = seq_cache_size + 1; k <= n; ++k) {
        seq_t val = compute_term(k);
        grow_seq_cache(seq_cache_size + 1);
        seq_cache[seq_cache_size++] = val;
        append_used(val);
    }
    return seq_cache[n - 1];
}
