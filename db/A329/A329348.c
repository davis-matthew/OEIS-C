/*
 * A329348  The least significant nonzero digit in the primorial base
 *          expansion of primorial inflation of n, A108951(n).
 * https://oeis.org/A329348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least significant nonzero digit of the primorial base
 *            expansion of A108951(n)
 * Range:     u8 n <= 3112, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *g_primes = NULL;
static size_t     g_prime_cnt = 0;
static int        g_prime_index[20001] = {0};

static void init_primes(void)
{
    if (g_primes != NULL) return;          /* already initialized */
    /* build primes up to an upper bound large enough for n <= 10000 */
    size_t limit = 20000;
    build_primes_upto(limit, &g_primes, &g_prime_cnt);
    for (size_t i = 0; i < g_prime_cnt; ++i)
        g_prime_index[(int)g_primes[i]] = (int)i;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

static uint64_t mod_inv_uint64(uint64_t a, uint64_t mod)
{
    /* mod is prime, use Fermat's little theorem */
    return mod_pow_uint64(a, mod - 2, mod);
}

/* Computes A329348(n) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* special case, A108951(1) = 1 */

    init_primes();

    /* factor n */
    uint64_t pf[16], exp[16];
    size_t cnt = 0;
    factor_u64(n, pf, exp, &cnt);         /* cnt <= number of distinct primes of n */

    /* iterate over primorial positions k = 1,2,... */
    for (size_t k = 1; k <= g_prime_cnt; ++k) {
        uint64_t pkprime = g_primes[k];           /* prime_{k+1} */
        /* compute pkmod = primorial_k mod pkprime = product of primes <= g_primes[k-1]  */
        uint64_t pkmod = 1 % pkprime;
        for (size_t i = 0; i <= k - 1; ++i) {
            pkmod = (pkmod * g_primes[i]) % pkprime;
        }
        uint64_t inv_pkmod = mod_inv_uint64(pkmod, pkprime);

        /* compute m mod pkprime: product over i of (primorial_{idx(pf[i])})^exp[i] mod pkprime */
        uint64_t m_mod = 1 % pkprime;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t prime_val = pf[i];
            int idx = g_prime_index[(int)prime_val];
            /* primorial of this prime modulo pkprime */
            uint64_t prim_mod = 1 % pkprime;
            for (int j = 0; j <= idx; ++j) {
                prim_mod = (prim_mod * g_primes[j]) % pkprime;
            }
            m_mod = (m_mod * mod_pow_uint64(prim_mod, exp[i], pkprime)) % pkprime;
        }

        uint64_t digit = (m_mod * inv_pkmod) % pkprime;
        if (digit != 0) return (seq_t)digit;
    }

    /* In theory we should always return inside the loop; fallback */
    return (seq_t)0;
}
