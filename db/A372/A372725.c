/*
 * A372725  a(n) = Sum_{k=2..n+2} L(n/prime(k)) where L(n/p) is the Legendre
 *          symbol. Row sums of A372726.
 * https://oeis.org/A372725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=2..n+2} L(n/prime(k)) where L is the Legendre
 *            symbol (a/p) with p an odd prime. The sequence is indexed with
 *            offset 0
 * Range:     i8 n <= 143, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- helper: modular exponentiation ---------- */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- helper: Legendre symbol (a/p) ---------- */
static int legendre_symbol(uint64_t a, uint64_t p)
{
    a %= p;
    if (a == 0)
        return 0;
    uint64_t exp = (p - 1) >> 1;
    uint64_t r   = powmod_uint64(a, exp, p);
    if (r == 1)
        return 1;
    if (r == p - 1)
        return -1;
    return 0; /* should not occur for odd prime p */
}

/* ---------- global prime table ---------- */
static uint64_t *prime_tbl      = NULL; /* primes[0] = 2, primes[1] = 3, ... */
static size_t   prime_tbl_cnt   = 0;     /* number of primes stored */

/* ---------- reseed prime table when needed ---------- */
static void ensure_primes(size_t needed)
{
    if (needed <= prime_tbl_cnt)
        return; /* already have enough primes */

    /* over‑estimate an upper bound for the needed‑th prime.
       For n>=6: p_n < n (log n + log log n).
       Using a simple factor 20 gives a comfortable limit for n<=10⁴. */
    size_t limit = needed * 20 + 10;
    size_t i, j, cnt = 0;

    /* sieve into a temporary boolean array */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime)
        return; /* allocation failure – silently keep old primes */
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= limit; ++i)
    {
        if (is_prime[i])
            for (j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
    }
    for (i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *new_tbl = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!new_tbl)
    {
        free(is_prime);
        return;
    }

    for (i = 2, j = 0; i <= limit; ++i)
        if (is_prime[i])
            new_tbl[j++] = i;

    free(is_prime);

    free(prime_tbl);
    prime_tbl      = new_tbl;
    prime_tbl_cnt  = cnt;
}

/* ---------- the sequence function ---------- */
seq_t a(idx_t n)
{
    /* We need primes up to prime(n+2). */
    size_t need = (size_t)(n) + 2;
    ensure_primes(need);

    long long sum = 0; /* long long is big enough for the expected sums */
    uint64_t n_u  = (uint64_t)n;

    for (size_t k = 2; k <= need; ++k) /* primes[0] = 2 */
    {
        uint64_t p = prime_tbl[k - 1];
        int s = legendre_symbol(n_u, p);
        sum += s;
    }
    return (seq_t)sum; /* cast at the final stage */
}
