/*
 * A337987  Odd numbers whose distinct prime indices are pairwise coprime,
 *          where a singleton is not considered coprime unless it is (1).
 * https://oeis.org/A337987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   “search” – generate odd numbers and test whether the set of
 *            distinct prime indices of the number are pairwise coprime, with
 *            the singleton case excluded. Returns the n‑th term (n ≥ 1)
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- sieve helpers ---------- */
static const idx_t LIMIT = 5000000;          /* enough for n≈10000 */
static uint32_t *spf = NULL;                 /* smallest prime factor */
static uint32_t *prime_index_arr = NULL;     /* 1‑based index of a prime, 0 otherwise */
static uint32_t *primes = NULL;              /* list of primes */
static size_t primes_cnt = 0;
static bool sieve_initialized = false;

/* cast to avoid C++ implicit conversion warning */
static void init_sieve(void)
{
    if (sieve_initialized) return;

    spf = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));
    prime_index_arr = (uint32_t *)calloc((size_t)LIMIT + 1, sizeof(uint32_t));

    /* generous capacity for primes; 5 M has < 350 000 primes */
    size_t primes_cap = 400000;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    primes_cnt = 0;

    for (uint32_t p = 2; p <= LIMIT; ++p) {
        if (!spf[p]) {                   /* p is prime */
            spf[p] = p;
            if (primes_cnt >= primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt] = p;
            prime_index_arr[p] = (uint32_t)(primes_cnt + 1); /* 1‑based */
            ++primes_cnt;

            if ((uint64_t)p * p <= LIMIT) {
                for (uint64_t m = (uint64_t)p * p; m <= LIMIT; m += p)
                    if (!spf[(size_t)m]) spf[(size_t)m] = p;
            }
        }
    }
    sieve_initialized = true;
}

/* gcd for uint32_t (needed for pairwise coprimality test) */
static uint32_t gcd32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* test whether an odd number satisfies the sequence condition */
static bool is_valid_odd(uint64_t v)
{
    if ((v & 1ULL) == 0) return false;          /* must be odd */

    uint64_t n = v;
    uint32_t last_p = 0;
    uint32_t idxs[64];
    size_t idx_cnt = 0;

    while (n > 1) {
        uint32_t p;
        if (n <= LIMIT) p = spf[(size_t)n];
        else {
            /* fallback trial division for rare large primes */
            uint32_t r = 2;
            for (; (uint64_t)r * r <= n; ++r)
                if (n % r == 0) { p = r; break; }
            p = n;
        }

        if (p != last_p) {
            if (idx_cnt >= sizeof(idxs)/sizeof(idxs[0])) return false;
            idxs[idx_cnt++] = prime_index_arr[p];
            last_p = p;
        }
        n /= p;
    }

    if (idx_cnt < 2) return false;          /* singleton not allowed */

    for (size_t i = 0; i < idx_cnt; ++i)
        for (size_t j = i + 1; j < idx_cnt; ++j)
            if (gcd32(idxs[i], idxs[j]) != 1) return false;

    return true;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    init_sieve();

    uint64_t candidate = 3;                /* first odd >1 */
    idx_t count = 0;

    while (1) {
        if (is_valid_odd(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 2;
    }
}
