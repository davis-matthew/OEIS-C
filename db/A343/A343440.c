/*
 * A343440  a(n) = Sum_{d|n, gcd(d, n/d) = 1} (-1)^omega(n/d) * 2^(d-1).
 * https://oeis.org/A343440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n)=2^(n-1)-∑_{d|n, gcd(d,n/d)=1, d<n} a(d)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   ensure_cache, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/ensure_cache.h"

/* Prime factorisation of a uint64_t (uses trial division).  */
static void factor_uint64(uint64_t n, uint64_t primes[], int exps[], size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1) {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Generates all positive divisors of a uint64_t number.  */
static void generate_divisors_uint64(uint64_t n, uint64_t divs[], size_t *dcnt)
{
    uint64_t primes[20];
    int     exps[20];
    size_t  pc;
    factor_uint64(n, primes, exps, &pc);

    size_t total = 1;
    divs[0] = 1;                            /* divisor 1 */
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p  = primes[i];
        int     e  = exps[i];
        size_t  old = total;
        uint64_t pk = 1;
        for (int k = 1; k <= e; ++k) {
            pk *= p;
            for (size_t j = 0; j < old; ++j)
                divs[total++] = divs[j] * pk;
        }
    }
    *dcnt = total;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* undefined below offset */
    ensure_cache(n + 1, &cache, &cache_cap);

    if (cache[n] != 0) return cache[n];           /* already computed */

    uint64_t num = (uint64_t)n;
    uint64_t divs[200];
    size_t   dcnt;
    generate_divisors_uint64(num, divs, &dcnt);

    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d == num) continue;                  /* skip n itself */
        if (gcd((seq_t)d, (seq_t)(num / d)) == 1)
            sum += a((idx_t)d);
    }

    seq_t res = pow2((idx_t)(num - 1)) - sum;
    cache[n] = res;
    return res;
}
