/*
 * A247574  Base-13 state complexity of partitioned deterministic finite
 *          automaton (PDFA) for the periodic sequence (123...n)*.
 * https://oeis.org/A247574
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n * ord_n(13) for n not divisible by 13; if 13 | n then
 *            a(n) = n + 1
 * Range:     u8 n <= 18, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static inline seq_t mod_pow_uint(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return (seq_t)res;
}

extern seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* For n divisible by 13 the order of 13 modulo n is undefined
       (13^k ≡ 0 (mod 13)), the sequence defines a(n)=n+1 */
    if (n % 13ULL == 0) return (seq_t)n + 1;

    /* ----- compute Euler's totient φ(n) -------------------------------- */
    uint64_t t = n;
    uint64_t phi = n;
    const uint32_t primes_small[] = {
        2,3,5,7,11,13,17,19,23,29,31,
        37,41,43,47,53,59,61,67,71,73,
        79,83,89,97
    };
    size_t p_cnt = sizeof(primes_small) / sizeof(primes_small[0]);
    for (size_t i = 0; i < p_cnt; ++i) {
        uint32_t p = primes_small[i];
        if ((uint64_t)p * p > t) break;
        if (t % p == 0) {
            phi -= phi / p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) phi -= phi / t;

    /* ----- factor φ(n) for order computation ----------------------------- */
    uint64_t phi_t = phi;
    uint32_t pf[24];
    unsigned int pe[24];
    size_t fac_cnt = 0;
    for (size_t i = 0; i < p_cnt; ++i) {
        uint32_t p = primes_small[i];
        if ((uint64_t)p * p > phi_t) break;
        if (phi_t % p == 0) {
            unsigned int e = 0;
            while (phi_t % p == 0) {
                phi_t /= p;
                ++e;
            }
            pf[fac_cnt] = p;
            pe[fac_cnt] = e;
            ++fac_cnt;
        }
    }
    if (phi_t > 1) {
        pf[fac_cnt] = (uint32_t)phi_t;
        pe[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* ----- find multiplicative order of 13 modulo n --------------------- */
    uint64_t order = phi;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint32_t p = pf[i];
        while (order % p == 0) {
            uint64_t cand = order / p;
            if (mod_pow_uint(13ULL, cand, n) == 1) {
                order = cand;
            } else {
                break;
            }
        }
    }

    return (seq_t)n * (seq_t)order;
}
