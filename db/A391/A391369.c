/*
 * A391369  Absolute values of norms of prime elements in Z[sqrt(3)].
 * https://oeis.org/A391369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th element of the set {2,3} ∪ {p prime | p≡1 or 11
 *            (mod 12)} ∪ {p² | p prime, p≡5 or 7 (mod 12)}. The
 *            implementation builds a sieve up to a fixed bound, then
 *            generates successive members until n terms are available. The
 *            sequence is cached for repeated calls
 * Range:     u8 n <= 29, u16 n <= 3299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- parameters ---------- */
#define MAX_VAL       310000           /* sieve bound sufficient for n≈10000 */
#define INITIAL_CAP   64

/* ---------- static data ---------- */
static uint8_t is_prime_sieve[MAX_VAL + 1];
static bool sieve_done = false;

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint32_t next_candidate = 2;

/* ---------- helpers ---------- */

/* Build sieve of Eratosthenes once */
static void build_sieve(void)
{
    if (sieve_done) return;
    for (uint32_t i = 0; i <= MAX_VAL; ++i) is_prime_sieve[i] = 1;
    is_prime_sieve[0] = is_prime_sieve[1] = 0;
    for (uint32_t i = 2; i * i <= MAX_VAL; ++i)
        if (is_prime_sieve[i])
            for (uint32_t j = i * i; j <= MAX_VAL; j += i)
                is_prime_sieve[j] = 0;
    sieve_done = true;
}

/* Integer square root for uint32_t */
static inline uint32_t isqrt_u32(uint32_t x)
{
    uint32_t lo = 0, hi = 65536;
    while (lo < hi) {
        uint32_t mid = lo + ((hi - lo + 1) >> 1);
        if ((uint64_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Ensure cache capacity (realloc with explicit cast for C++ compatibility) */
static inline void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t cap = cache_cap == 0 ? INITIAL_CAP : cache_cap * 2;
    if (cap < needed) cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);   /* memory failure */
    cache = tmp;
    cache_cap = cap;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    build_sieve();

    /* Generate terms until we reach the requested index */
    while (cache_len < n) {
        if (next_candidate > MAX_VAL) {
            /* For safety: if bound exceeded, increase is not supported */
            return 0;
        }

        /* Decision logic for membership */
        bool member = false;
        uint32_t v = next_candidate;
        if (v <= 3) {
            member = (v == 2 || v == 3);
        } else if (is_prime_sieve[v]) {
            uint32_t mod12 = v % 12;
            member = (mod12 == 1 || mod12 == 11);
        } else {
            uint32_t r = isqrt_u32(v);
            if ((uint64_t)r * r == v && r <= MAX_VAL && is_prime_sieve[r]) {
                uint32_t mod12 = r % 12;
                member = (mod12 == 5 || mod12 == 7);
            }
        }

        if (member) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)v;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
