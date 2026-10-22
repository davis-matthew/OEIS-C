/*
 * A359387  Primes p such that the smallest prime factor of (2^(p-1)-1)/(3*p)
 *          is greater than p.
 * https://oeis.org/A359387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A359387: primes p > 7 such that the smallest prime factor
   of (2^(p-1)-1)/(3p) is greater than p.  The first terms are listed
   in the OEIS entry; the function returns a(n) for any n >= 1. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- internal helpers ---------- */

#define LIMIT 12000000U   /* sieve limit – large enough for the first ~10 000 terms */

/* Simple modular exponentiation: base^exp mod mod (32‑bit). */
static uint32_t mod_pow(uint32_t base, uint32_t exp, uint32_t mod)
{
    uint32_t result = 1;
    while (exp) {
        if (exp & 1u)
            result = (uint32_t)((uint64_t)result * base % mod);
        base = (uint32_t)((uint64_t)base * base % mod);
        exp >>= 1u;
    }
    return result;
}

/* Multiplicative order of 2 modulo prime p (p > 2).  Uses spf array
   for fast factorisation of p-1. */
static uint32_t order_of_two(uint32_t p, const uint32_t *spf)
{
    uint32_t ord = p - 1;
    uint32_t t = ord;
    uint32_t prime_factor;
    int count;

    while (t > 1) {
        prime_factor = spf[t];
        count = 0;
        while (t % prime_factor == 0) {
            t /= prime_factor;
            ++count;
        }
        for (int i = 0; i < count; ++i) {
            if (mod_pow(2u, ord / prime_factor, p) == 1u)
                ord /= prime_factor;
            else
                break;
        }
    }
    return ord;
}

/* Generate all divisors of n (n ≥ 1).  divs must be large enough
   (for n <= LIMIT the divisor count is <= 256).  Returns the number
   of divisors. */
static size_t generate_divisors(uint32_t n,
                                const uint32_t *spf,
                                uint32_t divs[],
                                size_t max_divs)
{
    uint32_t primes[32];
    int exp[32];
    int len = 0;
    uint32_t t = n;

    while (t > 1) {
        uint32_t p = spf[t];
        int c = 0;
        while (t % p == 0) {
            t /= p;
            ++c;
        }
        primes[len] = p;
        exp[len] = c;
        ++len;
    }

    divs[0] = 1u;
    size_t count = 1u;
    for (int i = 0; i < len; ++i) {
        uint32_t prime = primes[i];
        int e = exp[i];
        size_t current = count;
        uint32_t mul = 1u;
        for (int j = 1; j <= e; ++j) {
            mul *= prime;
            for (size_t k = 0; k < current; ++k) {
                if (count >= max_divs) return count; /* safety */
                divs[count++] = divs[k] * mul;
            }
        }
    }
    return count;
}

/* ---------- global data for lazy initialization ---------- */

static uint32_t *spfs = NULL;            /* smallest prime factor for each number */
static uint32_t *minPrimeOrder = NULL;   /* smallest prime achieving a given order */
static uint32_t *primes = NULL;          /* list of primes up to LIMIT */
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static seq_t *seq_vals = NULL;           /* sequence values */
static size_t seq_cnt = 0;
static size_t seq_cap = 0;
static bool initialized = false;

/* Precomputation: sieve, orders, sequence generation */
static void preprocess(void)
{
    if (initialized)
        return;

    /* ---------- linear sieve for spfs and primes ---------- */
    const uint32_t N = LIMIT;
    spfs = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    primes_cap = N / 10 + 10;               /* rough initial capacity */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));

    for (uint32_t i = 2; i <= N; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = i;
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > N) break;
            spfs[(uint32_t)prod] = p;
            if (p == spfs[i]) break;
        }
    }

    /* ---------- build mapping: order → smallest prime achieving it ---------- */
    minPrimeOrder = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t q = primes[i];
        if (q <= 3) continue;          /* 2 and 3 are not needed as witnesses */

        uint32_t ord = order_of_two(q, spfs);
        if (ord == 0 || ord > N) continue;

        if (minPrimeOrder[ord] == 0 || q < minPrimeOrder[ord])
            minPrimeOrder[ord] = q;
    }

    /* ---------- generate the sequence ---------- */
    seq_cap = 10000;
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    uint32_t div_storage[512];

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if (p <= 7) continue;          /* skip small primes – first term is 11 */

        /* factor p-1 and produce all its divisors */
        size_t div_cnt = generate_divisors(p - 1, spfs, div_storage, 512);
        bool ok = true;
        for (size_t d_idx = 0; d_idx < div_cnt; ++d_idx) {
            uint32_t d = div_storage[d_idx];
            if (d <= 1) continue;
            uint32_t q = minPrimeOrder[d];
            if (q == 0 || q >= p || q == 3) continue;
            ok = false;                 /* found a small factor <= p */
            break;
        }
        if (ok) {
            if (seq_cnt == seq_cap) {
                seq_cap *= 2;
                seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_cnt++] = (seq_t)p;
        }
    }

    initialized = true;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0 – return 0 for safety */

    preprocess();

    if ((idx_t)seq_cnt < n)
        return 0; /* requested term beyond precomputed range */

    return seq_vals[(size_t)(n - 1)];
}
