/*
 * A371699  The smallest composite number which divides the concatenation of
 *          its descending ordered prime factors, with repetition, when
 *          written in base n.
 * https://oeis.org/A371699
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest composite number m such that * m divides
 *            the concatenation of the prime factors of m in * descending
 *            order, written in base n. * For odd n (≥3) a(n)=4; for n=6k+2
 *            (k>0) a(n)=9; otherwise the * value is found by brute force
 *            search
 * Range:     u8 n <= 5, u16 n <= 227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   passes_concatenation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/passes_concatenation.h"

/* ---------- prime sieve ---------- */

#define PRIME_LIMIT 200000              /* enough to factor numbers long
                                           enough for n≤10000           */

static bool prime_mark[PRIME_LIMIT + 1];
static uint32_t primes[ PRIME_LIMIT / 10 ];  /* rough upper bound on count */
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;
    memset(prime_mark, 0, sizeof(prime_mark));
    prime_mark[0] = prime_mark[1] = true;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!prime_mark[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                prime_mark[j] = true;
        }
    }
    size_t idx = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (!prime_mark[i]) primes[idx++] = (uint32_t)i;
    prime_cnt = idx;
    primes_built = true;
}

/* ---------- factorisation (ascending prime factors) ---------- */

static void factor_uint64(uint64_t m,
                           uint64_t *factors, int *cnt)
{
    int c = 0;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > temp) break;
        while (temp % p == 0) {
            factors[c++] = p;
            temp /= p;
        }
    }
    if (temp > 1) factors[c++] = temp;
    *cnt = c;
}

/* ---------- test divisibility of concatenation by m ---------- */

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n<2 */

    /* quick cases */
    if (n == 2) return (seq_t)42;
    if ((n & 1ULL) && n >= 3) return (seq_t)4;           /* odd n */
    if ((n % 6ULL) == 2ULL && n > 2ULL) return (seq_t)9; /* 6k+2, k>0 */

    /* general case (even n that is not 6k+2) */
    build_primes();
    const uint32_t base_n = (uint32_t)n;
    uint64_t factors[64];
    int factor_cnt;

    for (uint64_t m = 4; ; ++m) {
        factor_uint64(m, factors, &factor_cnt);
        if (factor_cnt <= 1) continue;                         /* skip primes */
        if (passes_concatenation(m, base_n, factors, factor_cnt))
            return (seq_t)m;
    }
}
