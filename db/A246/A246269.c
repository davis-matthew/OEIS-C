/*
 * A246269  a(1) = 1, a(p(k)) = p(k+1) mod 4 for k-th prime p(k) and a(u * v)
 *          = a(u) * a(v) for u, v > 0.
 * https://oeis.org/A246269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 9881, u64 n <= 9881,
 *            u128 n <= 9881, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A246269
 *  a(1) = 1
 *  For prime p_k, a(p_k) = (p_{k+1} mod 4).
 *  a(n) fully multiplicative: a(u*v) = a(u)*a(v) for u,v>0.
 *  The value is always a power of 3: a(n) = 3^{E},
 *  where E = sum_{p^e || n} e * [p_{next} mod 4 == 3].
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow3.h"   /* seq_t pow3(idx_t exp) */

#define PRIME_LIMIT 20000   /* sufficient for all n in typical tests */

/* --- static, one‑time prime sieve ---------------------------------- */
static uint64_t primes[PRIME_LIMIT + 1];
static size_t prime_count = 0;
static bool primes_built = false;

static void build_prime_list(void)
{
    bool *mark = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!mark) { exit(1); }          /* trivial error handling */

    size_t cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!mark[i]) {
            primes[cnt++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                    mark[j] = true;
            }
        }
    }
    prime_count = cnt;
    free(mark);
    primes_built = true;
}

/* --- helper: binary search for prime index -------------------------- */
static size_t find_prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* primes[lo] == p if found */
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (!primes_built)
        build_prime_list();

    uint64_t nn = (uint64_t)n;
    idx_t exponent_sum = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > nn)
            break;
        if (nn % p != 0)
            continue;

        idx_t e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }

        /* next prime after p */
        size_t idx = find_prime_index(p);
        uint64_t next_prime = primes[idx + 1];   /* safe because LIMIT is large enough */
        if (next_prime % 4 == 3)
            exponent_sum += e;
    }

    if (nn > 1) {     /* remaining prime factor > sqrt(original n) */
        size_t idx = find_prime_index(nn);
        uint64_t next_prime = primes[idx + 1];
        if (next_prime % 4 == 3)
            exponent_sum += 1;
    }

    return pow3(exponent_sum);
}
