/*
 * A342607  a(n) = Sum_{d|n} phi(d)^(n-d).
 * https://oeis.org/A342607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n-d)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 32, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* Recursive helper to enumerate all divisors of n */
static void gen_divisors(size_t pos, uint64_t cur,
                         const uint64_t *pf, const uint64_t *exps,
                         size_t pf_cnt, uint64_t *out, size_t *out_cnt)
{
    if (pos == pf_cnt) {
        out[*out_cnt] = cur;
        (*out_cnt)++;
        return;
    }
    uint64_t p = pf[pos];
    uint64_t val = 1;
    for (unsigned e = 0; e <= exps[pos]; ++e) {
        gen_divisors(pos + 1, cur * val, pf, exps, pf_cnt, out, out_cnt);
        val *= p;
    }
}

/* Ensure a small static list of primes for factorization */
static bool primes_inited = false;
static uint64_t *global_primes = NULL;
static size_t global_prime_cnt = 0;
static void ensure_primes(void)
{
    if (!primes_inited) {
        idx_t limit = 100000;                   /* enough for n up to ~10^4 */
        build_primes_upto(limit, &global_primes, &global_prime_cnt);
        primes_inited = true;
    }
}

/* Main sequence function */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, return 0 for safety */

    ensure_primes();

    /* Factor n */
    uint64_t temp = (uint64_t)n;
    uint64_t pf[20];    /* primes */
    uint64_t exps[20];  /* exponents */
    size_t pf_cnt = 0;

    for (size_t i = 0; i < global_prime_cnt &&
                         (uint64_t)global_primes[i] * (uint64_t)global_primes[i] <= temp;
         ++i) {
        uint64_t p = global_primes[i];
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            pf[pf_cnt] = p;
            exps[pf_cnt] = e;
            pf_cnt++;
        }
    }
    if (temp > 1) {
        pf[pf_cnt] = temp;
        exps[pf_cnt] = 1;
        pf_cnt++;
    }

    /* Generate all divisors */
    uint64_t divisors[512];
    size_t div_cnt = 0;
    gen_divisors(0, 1, pf, exps, pf_cnt, divisors, &div_cnt);

    /* Sum the terms */
    seq_t result = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divisors[i];
        seq_t phi_d = phi((idx_t)d);
        seq_t term = big_pow(phi_d, (seq_t)(n - d));
        result += term;
    }
    return result;
}
