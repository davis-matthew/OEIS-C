/*
 * A339100  a(n) = GCD({(2*n-k)*T(n,k)+(k+1)*T(n,k+1), k=0..n}), where T(n,k)
 *          stands for A214406 (the second-order Eulerian numbers of type B).
 * https://oeis.org/A339100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^v2(n) * ∏_{p prime, p-1|n} p^e_p, where e_p = 1 if p=3
 *            and 2|n, e_p = 2 if p≥5, p-1|n and (n/(p-1))%p==0, e_p = 1
 *            otherwise. (a(odd n) = 1)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 119, u64 n <= 419,
 *            u128 n <= 1201, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PRIME_CAP 200000   /* sufficient for n up to ~1e5 */

static bool primes_inited = false;
static size_t prime_count = 0;
static uint64_t primes[PRIME_CAP];

static void init_primes(void)
{
    if (primes_inited) return;
    bool is_comp[PRIME_CAP];
    for (size_t i = 0; i < PRIME_CAP; ++i) is_comp[i] = false;
    is_comp[0] = is_comp[1] = true;
    for (size_t i = 2; i < PRIME_CAP; ++i) {
        if (!is_comp[i]) {
            primes[prime_count++] = (uint64_t)i;
            if (i * i < PRIME_CAP) {
                for (size_t j = i * i; j < PRIME_CAP; j += i)
                    is_comp[j] = true;
            }
        }
    }
    primes_inited = true;
}

seq_t a(idx_t n)
{
    /* The problem statement allows n starting from 1; if n==0, return 0. */
    if (n == 0) return (seq_t)0;

    init_primes();

    seq_t result = (seq_t)1;
    /* v2(n) */
    size_t v2 = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0ULL) {
        ++v2;
        tmp >>= 1;
    }
    for (size_t i = 0; i < v2; ++i)
        result *= (seq_t)2;

    /* odd part of n */
    idx_t odd_part = tmp;   /* n after removing all factors of 2 */

    /* For every prime p ≥ 3 with p-1 | n */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p < 3ULL) continue;
        uint64_t pm1 = p - 1ULL;
        if (pm1 > (uint64_t)n) break;
        if ((uint64_t)n % pm1 != 0ULL) continue;
        /* compute exponent e_p */
        unsigned exp = 1;
        if (p == 3ULL) {
            /* e_3 is 1 iff 2|n; we already know n even (since we removed 2's), so keep 1 */
        } else { /* p >= 5 */
            idx_t t = n / pm1;   /* integer division */
            if (t % p == 0ULL)
                exp = 2;
        }
        for (unsigned k = 0; k < exp; ++k)
            result *= (seq_t)p;
    }
    return result;
}
