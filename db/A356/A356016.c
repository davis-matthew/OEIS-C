/*
 * A356016  Consider the exponents in the prime factorization of n, and
 *          replace each run of k consecutive e's by a unique k; the resulting
 *          list corresponds to the exponents in the prime factorization of
 *          a(n).
 * https://oeis.org/A356016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A319522(A356008(n)) (interpretation: run‑length encode
 *            the exponents of n and use those run lengths as exponents of the
 *            smallest primes)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64 */

/* fast power for seq_t */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != 0) {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* --- factorise n --------------------------------------------- */
    uint64_t pf[64];          /* prime factors */
    uint64_t ef[64];          /* corresponding exponents */
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pf, ef, &pf_cnt);
    if (pf_cnt == 0) return (seq_t)1;   /* impossible for n>1 */

    uint64_t max_prime = pf[pf_cnt - 1];

    /* --- build list of primes up to max_prime --------------------- */
    /* upper bound on count of primes <= max_prime is < max_prime/2
       for max_prime <= 10^6 this is fine. Here n <= 10000, so even
       a very small buffer is enough. */
    size_t prime_cap = max_prime / 2 + 4;
    uint64_t *primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    if (!primes) return (seq_t)1;   /* allocation failure fallback */

    size_t primes_cnt = 0;
    /* prime 2 */
    primes[primes_cnt++] = 2;
    for (uint64_t candidate = 3; candidate <= max_prime; candidate += 2) {
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            if (primes_cnt + 1 > prime_cap) {
                prime_cap = prime_cap * 2 + 4;
                uint64_t *tmp = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                if (!tmp) { free(primes); return (seq_t)1; }
                primes = tmp;
            }
            primes[primes_cnt++] = candidate;
        }
    }

    /* --- run‑length encode the exponent list ----------------------- */
    seq_t result = (seq_t)1;
    size_t pf_idx = 0;
    int last_exp = -1;
    size_t run_len = 0;
    size_t run_prime_idx = 0;   /* index into primes array for exponent */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = 0;
        if (pf_idx < pf_cnt && pf[pf_idx] == p) {
            exp = ef[pf_idx++];
        }

        if (run_len == 0) {
            last_exp = (int)exp;
            run_len = 1;
        } else if ((int)exp == last_exp) {
            ++run_len;
        } else {
            /* finish current run */
            seq_t base = (seq_t)primes[run_prime_idx++];
            seq_t pow_val = seq_pow(base, (idx_t)run_len);
            result *= pow_val;

            last_exp = (int)exp;
            run_len = 1;
        }
    }

    /* process final run if any */
    if (run_len > 0 && run_prime_idx < primes_cnt) {
        seq_t base = (seq_t)primes[run_prime_idx];
        seq_t pow_val = seq_pow(base, (idx_t)run_len);
        result *= pow_val;
    }

    free(primes);
    return result;
}
