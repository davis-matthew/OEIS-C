/*
 * A333174  a(n) = Sum_{k=0..n} r_4(k^2 + 1), where r_4(k) is the number of
 *          ways of writing k as a sum of 4 squares (A000118).
 * https://oeis.org/A333174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} r_4(k^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1049, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* r_4(m) = 8 * sum_{d|m, 4∤d} d.
   Only the parity of the power of 2 in m matters. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve, built once ---------- */
static uint64_t *primes = NULL;       /* array of primes up to primes_limit */
static size_t primes_count = 0;       /* number of primes in the array   */
static uint64_t primes_limit = 0;     /* current sieve limit             */

/* simple integer sqrt for small values (limit <= 1e7) */
static uint64_t int_sqrt(uint64_t v)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= v) r++;
    return r;
}

/* rebuild the prime list up to 'limit' (inclusive) */
static void rebuild_primes(uint64_t limit)
{
    bool *is_prime = (bool*)malloc(limit + 1);
    if (!is_prime) exit(1);
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    uint64_t sq = int_sqrt(limit);
    for (uint64_t i = 2; i <= sq; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) cnt++;

    uint64_t *new_pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_pr) exit(1);
    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_pr[pos++] = i;

    free(primes);
    primes = new_pr;
    primes_count = cnt;
    primes_limit = limit;
    free(is_prime);
}

static void ensure_primes(uint64_t needed)
{
    if (primes_limit >= needed) return;
    rebuild_primes(needed);
}

/* ---------- r_4 computation ---------- */
static seq_t r4_of(uint64_t m)
{
    uint64_t temp = m;
    uint64_t a = 0;                     /* exponent of 2 in m */
    while ((temp & 1ULL) == 0 && temp > 1) {
        temp >>= 1;
        a++;
    }
    uint64_t odd = temp;                 /* odd part of m */

    seq_t sigma_odd = 1;                 /* product of (p^{e+1}-1)/(p-1) for odd primes */

    for (size_t i = 0; i < primes_count && primes[i] * primes[i] <= odd; ++i) {
        uint64_t p = primes[i];
        if ((odd % p) != 0) continue;
        uint64_t e = 0;
        while ((odd % p) == 0) {
            odd /= p;
            e++;
        }
        /* compute (p^{e+1}-1)/(p-1) */
        seq_t p_pow = 1;
        for (uint64_t j = 0; j < e + 1; ++j) p_pow = p_pow * (seq_t)p;
        seq_t term = (p_pow - 1) / (p - 1);
        sigma_odd = sigma_odd * term;
    }
    if (odd > 1) {
        /* remaining prime factor with exponent 1 */
        sigma_odd = sigma_odd * (odd + 1);
    }

    seq_t r4 = 0;
    if (a == 0)
        r4 = (seq_t)8 * sigma_odd;   /* m is odd */
    else
        r4 = (seq_t)24 * sigma_odd;  /* a >= 1  (a == 1 or a >= 2 give same factor) */
    return r4;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* primes up to n+1 are enough for all m = k^2+1 with k <= n */
    ensure_primes((uint64_t)n + 1);

    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        uint64_t m = (uint64_t)k * k + 1;
        seq_t r4 = r4_of(m);
        total += r4;
    }
    return total;
}
