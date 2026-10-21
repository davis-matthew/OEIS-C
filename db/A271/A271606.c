/*
 * A271606  Number of numbers k such that sigma_e(k) = n for some e > 0.
 * https://oeis.org/A271606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1187, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Factory: A271606 – Number of numbers k such that σₑ(k) = n for some e > 0
   (OEIS notation).  The function implements a direct search of all k ≤ n
   and all exponents e such that σₑ(k) ≤ n. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/big_pow.h"

static unsigned long long primes[20000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Simple sieve to fill the primes array up to LIMIT. */
static void build_primes_up_to(void) {
    const unsigned LIMIT = 100000;          /* sufficient for all calls up to n≈10⁴ */
    bool *is_composite = (bool*)calloc((size_t)LIMIT + 1, 1);
    for (unsigned i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[prime_cnt++] = i;
            if ((unsigned long long)i * i <= LIMIT)
                for (unsigned j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = 1;
        }
    }
    free(is_composite);
    primes_ready = true;
}

/* Compute σₑ(k) given prime factorisation of k. */
static seq_t sigma_of_e(idx_t k, idx_t e,
                        const unsigned long long *pf, const unsigned char *ex, size_t pf_cnt)
{
    seq_t total = (seq_t)1;
    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t base = (seq_t)pf[i];
        seq_t exp_num = (seq_t)(e * (ex[i] + 1));
        seq_t num = big_pow(base, exp_num) - (seq_t)1;
        seq_t denom = big_pow(base, (seq_t)e) - (seq_t)1;
        seq_t factor = num / denom;
        total *= factor;
    }
    return total;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!primes_ready) build_primes_up_to();

    seq_t count = (seq_t)0;
    seq_t val_n = (seq_t)n;

    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {
            if (val_n == (seq_t)1) count += (seq_t)1;
            continue;
        }

        /* Factorise k. */
        unsigned long long pf[16];
        unsigned char ex[16];
        size_t pf_cnt = 0;
        unsigned long long tmp = (unsigned long long)k;

        for (size_t j = 0; j < prime_cnt && primes[j]*(unsigned long long)primes[j] <= tmp; ++j) {
            if (tmp % primes[j] == 0) {
                unsigned char c = 0;
                while (tmp % primes[j] == 0) { tmp /= primes[j]; ++c; }
                if (pf_cnt < sizeof(pf)/sizeof(pf[0])) {
                    pf[pf_cnt] = primes[j];
                    ex[pf_cnt] = c;
                    ++pf_cnt;
                }
            }
        }
        if (tmp > 1 && pf_cnt < sizeof(pf)/sizeof(pf[0])) {
            pf[pf_cnt] = tmp;
            ex[pf_cnt] = 1;
            ++pf_cnt;
        }

        /* Determine maximal exponent e to try: k^e ≤ n. */
        idx_t emax = 0;
        unsigned long long pow_k = 1;
        while (pow_k <= val_n / (unsigned long long)k) {
            pow_k *= (unsigned long long)k;
            ++emax;
        }

        for (idx_t e = 1; e <= emax; ++e) {
            seq_t sigma = sigma_of_e(k, e, pf, ex, pf_cnt);
            if (sigma == val_n) {
                ++count;
                break;        /* count each k once */
            }
            if (sigma > val_n) break;      /* subsequent e give larger values */
        }
    }
    return count;
}
