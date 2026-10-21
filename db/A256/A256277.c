/*
 * A256277  C(2*n,n) mod 2*n+1.
 * https://oeis.org/A256277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) mod (2*n+1)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* global prime table */
static int  *global_primes  = NULL;
static size_t global_prime_count = 0;
static idx_t  primes_limit = 0;

/* ensure primes <= limit are pre‑computed */
static void init_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                     /* already computed up to this limit */

    /* new sieve */
    size_t size = (size_t)limit + 1;
    bool *is_comp = (bool*)malloc(size);
    if (!is_comp)
        abort();
    memset(is_comp, 0, size);

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_comp[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++cnt;

    int *pr = (int*)malloc(cnt * sizeof(int));
    if (!pr)
        abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            pr[idx++] = (int)i;

    free(global_primes);
    global_primes = pr;
    global_prime_count = cnt;
    primes_limit = limit;

    free(is_comp);
}

/* modular exponentiation for uint64_t */
static uint64_t mod_pow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* offset starts at n = 1 */

    idx_t m = 2 * n + 1;               /* modulus */
    init_primes(2 * n);                /* primes up to 2n */

    uint64_t res = 1 % m;

    for (size_t k = 0; k < global_prime_count; ++k) {
        int p = global_primes[k];
        if ((idx_t)p > 2 * n)
            break;

        /* exponent of p in C(2n,n) */
        idx_t exp = 0;
        idx_t tmp;
        tmp = 2 * n;
        while (tmp) {
            tmp /= p;
            exp += tmp;
        }
        tmp = n;
        while (tmp) {
            tmp /= p;
            exp -= 2 * tmp;
        }

        if (exp) {
            uint64_t term = mod_pow_uint64((uint64_t)p, exp, m);
            res = (res * term) % m;
        }
    }
    return (seq_t)res;
}
