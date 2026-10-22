/*
 * A393054  Number of divisors d of n such that d and A276086(n) are not
 *          coprime, where A276086 is the primorial base exp-function.
 * https://oeis.org/A393054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Number of divisors d of n such that gcd(d, A276086(n)) >
 *            1 * where A276086(n) is the primorial base exp‑function. *
 *            Implementation: for each n factorise n, compute * tau(n) = total
 *            number of divisors * tau_coprime = number of divisors of n
 *            coprime to A276086(n) * a(n) = tau(n) - tau_coprim...
 * Range:     u8 n <= 9424, u16 n <= 9424, u32 n <= 9424, u64 n <= 9424,
 *            u128 n <= 9424, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

/* ------------------------------------------------------------------
 * small list of primes up to 200; enough for all operations
 * ------------------------------------------------------------------ */
static const uint64_t small_primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67,
    71, 73, 79, 83, 89, 97, 101, 103, 107,
    109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193,
    197, 199
};
#define SMALL_PRIME_COUNT (sizeof(small_primes)/sizeof(small_primes[0]))

/* ------------------------------------------------------------------
 * Helper: factorise n using primes in small_primes.
 * On return: cnt_out is number of distinct prime factors,
 * out_primes[i] holding factor, out_exps[i] its exponent.
 * ------------------------------------------------------------------ */
static size_t
factor_uint64(uint64_t n, uint64_t *out_primes, uint64_t *out_exps,
              size_t *cnt_out)
{
    size_t cnt = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < SMALL_PRIME_COUNT && small_primes[i]*small_primes[i] <= tmp; ++i) {
        uint64_t p = small_primes[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            out_primes[cnt] = p;
            out_exps[cnt]   = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                    /* remaining prime factor */
        out_primes[cnt] = tmp;
        out_exps[cnt]   = 1;
        ++cnt;
    }
    *cnt_out = cnt;
    return cnt;
}

/* ------------------------------------------------------------------
 * Primality table of primorials up to 10_000 (needed for A276086)
 * ------------------------------------------------------------------ */
static uint64_t primorials[10];      /* enough for 10 000 */
static size_t primorial_count;       /* number of entries stored */

/* ------------------------------------------------------------------
 * Initialise primorials: compute product of first k primes until value
 * exceeds the global maximum needed (10 000).  Primorial 0  = 1.
 * ------------------------------------------------------------------ */
static void
init_primorials(void)
{
    if (primorial_count != 0) return;    /* already initialised */
    primorial_count = 0;
    primorials[primorial_count++] = 1;    /* P0 = 1 */
    uint64_t prod = 1;
    for (size_t i = 0; i < SMALL_PRIME_COUNT; ++i) {
        if (prod > UINT64_MAX / small_primes[i]) break;  /* avoid overflow */
        prod *= small_primes[i];
        if (prod > 10000ULL) {
            primorials[primorial_count++] = prod;    /* include the first >10000 */
            break;
        }
        primorials[primorial_count++] = prod;
    }
}

/* ------------------------------------------------------------------
 * Main function: a(idx_t n)
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* initialise global tables on first call */
    init_primorials();

    uint64_t nn = (uint64_t)n;
    if (nn == 0) return (seq_t)0;   /* undefined for n=0, keep safe */

    /* factorise n */
    uint64_t fac_primes[32];
    uint64_t fac_exps[32];
    size_t fac_cnt;
    factor_uint64(nn, fac_primes, fac_exps, &fac_cnt);

    /* total number of divisors tau(n) */
    seq_t tau = 1;
    for (size_t i = 0; i < fac_cnt; ++i)
        tau *= (seq_t)(fac_exps[i] + 1);

    /* digits of n in primorial base */
    bool prime_in_S[SMALL_PRIME_COUNT] = {false};
    uint64_t rem = nn;
    /* find largest primorial <= rem */
    size_t k = primorial_count - 1;
    while (k > 0 && primorials[k] > rem) --k;
    for (; k > 0; --k) {
        uint64_t digit = rem / primorials[k];
        if (digit != 0) {
            /* prime corresponding to this digit is small_primes[k] */
            /*  (since primorials[k] = product of first k primes) */
            prime_in_S[k] = true;
        }
        rem -= digit * primorials[k];
    }

    /* number of divisors coprime to A276086(n) */
    seq_t tau_coprime = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_primes[i];
        /* locate prime index in small_primes array */
        size_t pi = 0;
        while (pi < SMALL_PRIME_COUNT && small_primes[pi] < p) ++pi;
        if (pi < SMALL_PRIME_COUNT && small_primes[pi] == p) {
            if (!prime_in_S[pi]) {
                tau_coprime *= (seq_t)(fac_exps[i] + 1);
            }
        } else {
            /* prime not found in table – should not happen for n<=10000 */
            tau_coprime *= (seq_t)(fac_exps[i] + 1);
        }
    }

    return tau - tau_coprime;
}
