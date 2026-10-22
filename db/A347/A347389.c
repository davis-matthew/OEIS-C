/*
 * A347389  Dirichlet convolution of A003415(n) and A003415(A276086(n)), where
 *          A003415(n) is the arithmetic derivative of n, and A276086(n) gives
 *          the prime product form of primorial base expansion of n.
 * https://oeis.org/A347389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Σ_{d|n} A003415(n/d) * A327860(d) where A003415(m) is the
 *            arithmetic derivative of m and A327860(d) = A003415(A276086(d))
 *            is the derivative of the primorial base expansion of d
 * Range:     u8 n <= 29, u16 n <= 115, u32 n <= 777, u64 n <= 2309,
 *            u128 n <= 2309, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

/* --------------------------------------------------------------------
   Prime table and primorial table (generated once on first call)
   -------------------------------------------------------------------- */
#define MAX_PRIME_CNT 20000   /* enough for n up to about 10^7          */
static uint64_t prime_list[MAX_PRIME_CNT];
static size_t prime_cnt = 0;

static uint64_t primorial_list[MAX_PRIME_CNT];
static size_t primorial_cnt = 0;

static bool tables_initialized = false;

static void init_tables(void)
{
    if (tables_initialized) return;

    /* simple sieve up to a fixed limit */
    const uint64_t SIEVE_LIMIT = 200000;            /* > 20000 primes */
    bool *sieve = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    for (uint64_t i = 0; i <= SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = false;
    for (uint64_t i = 2; i <= SIEVE_LIMIT && prime_cnt < MAX_PRIME_CNT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = i;

    /* primorials: P[0]=1, P[i+1]=P[i]*prime_list[i]           */
    primorial_list[0] = 1;
    primorial_cnt = 1;
    unsigned __int128 prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        prod *= prime_list[i];
        if (prod > (unsigned __int128)UINT64_MAX) break;
        primorial_list[primorial_cnt++] = (uint64_t)prod;
    }

    free(sieve);
    tables_initialized = true;
}

/* --------------------------------------------------------------------
   Arithmetic derivative of an unsigned integer m (m <= 2^64-1)
   -------------------------------------------------------------------- */
static seq_t derivative_uint(uint64_t m)
{
    if (m <= 1) return 0;
    seq_t sum = 0;
    uint64_t tmp = m;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            sum += (seq_t)e * ((seq_t)m / p);
        }
    }
    if (tmp > 1) {                      /* remaining prime factor */
        sum += 1 * ((seq_t)m / tmp);
    }
    return sum;
}

/* --------------------------------------------------------------------
   Derivative of A276086(d) i.e. A327860(d)
   -------------------------------------------------------------------- */
static seq_t derivative_A276086(uint64_t d)
{
    if (d == 0) return 0;

    /* find largest k with primorial_list[k] <= d */
    size_t k = 0;
    while (k + 1 < primorial_cnt && primorial_list[k + 1] <= d) ++k;

    uint64_t rem = d;
    uint64_t digs[64] = {0};

    /* greedy primorial base expansion : digits[i] * primorial[i]   */
    for (size_t i = k + 1; i-- > 0; ) {
        uint64_t denom = primorial_list[i];
        digs[i] = rem / denom;
        rem -= digs[i] * denom;
    }

    /* compute product N = Π primes[i]^digs[i] */
    seq_t N = 1;
    for (size_t i = 0; i <= k; ++i) {
        if (digs[i]) {
            seq_t p_pow = seq_pow((seq_t)prime_list[i], (idx_t)digs[i]);
            N *= p_pow;
        }
    }

    /* derivative = Σ e_i * (N / p_i) */
    seq_t der = 0;
    for (size_t i = 0; i <= k; ++i) {
        if (digs[i]) {
            seq_t N_div_p = N / prime_list[i];
            der += (seq_t)digs[i] * N_div_p;
        }
    }

    return der;
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    seq_t total = 0;

    /* iterate over all divisors d of n */
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        uint64_t d1 = i;
        uint64_t d2 = n / i;

        /* term for divisor d1 */
        seq_t der1 = derivative_uint(n / d1);
        seq_t der2 = derivative_A276086(d1);
        total += der1 * der2;

        if (d1 != d2) {
            /* term for the paired divisor d2 */
            seq_t der3 = derivative_uint(n / d2);
            seq_t der4 = derivative_A276086(d2);
            total += der3 * der4;
        }
    }
    return total;
}
