/*
 * A294882  Number of proper divisors of n that are not irreducible when their
 *          binary expansion is interpreted as polynomial over GF(2).
 * https://oeis.org/A294882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d<n} (1 – A091225(d)) A091225(d) = 1 iff the
 *            binary representation of d is an irreducible polynomial over
 *            GF(2), otherwise 0
 * Range:     u8 n <= 9844, u16 n <= 9844, u32 n <= 9844, u64 n <= 9844,
 *            u128 n <= 9844, big any n
 * Helpers:   bitlen_uint32, poly_remainder
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_uint32.h"
#include "../utils/poly_remainder.h"

/* ---------- helpers ---------- */

/* test if the binary representation of d is an irreducible polynomial over GF(2) */
static bool is_irreducible(uint32_t d)
{
    if (d <= 1) return false;            /* constant or zero polynomial */
    unsigned int deg_d = bitlen_uint32(d) - 1;
    for (unsigned int deg_h = 1; deg_h < deg_d; ++deg_h) {
        unsigned int leading = (1U << deg_h);
        unsigned int range  = leading << 1;       /* 2^(deg_h+1) */
        for (unsigned int h = leading; h < range; ++h) {
            unsigned int rem = poly_remainder(d, h, deg_d, deg_h);
            if (rem == 0) return false;            /* reducible */
        }
    }
    return true;                               /* irreducible */
}

/* ---------- precomputation of irreducibility for all values up to MAX_N ---------- */
#define MAX_N 10000
static unsigned char irreducible[MAX_N + 1];
static bool irr_initialised = false;
static void init_irreducibility(void)
{
    if (irr_initialised) return;
    irr_initialised = true;
    irreducible[0] = 0;
    irreducible[1] = 0;
    for (uint32_t d = 2; d <= MAX_N; ++d)
        irreducible[d] = is_irreducible(d) ? 1 : 0;
}

/* ---------- prime table up to MAX_N ---------- */
static unsigned primes[1229];     /* number of primes <= 10000 is 1229 */
static size_t primes_cnt = 0;
static bool primes_initialised = false;
static void init_primes(void)
{
    if (primes_initialised) return;
    primes_initialised = true;
    static bool is_prime[10001];
    for (int i = 2; i <= 10000; ++i) is_prime[i] = true;
    for (int p = 2; (long long)p * p <= 10000; ++p)
        if (is_prime[p])
            for (int q = p * p; q <= 10000; q += p) is_prime[q] = false;
    for (int i = 2; i <= 10000; ++i)
        if (is_prime[i]) primes[primes_cnt++] = i;
}

/* ---------- divisor enumeration ---------- */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            pr[*cnt] = p;
            ex[*cnt] = e;
            ++(*cnt);
        }
    }
    if (rem > 1) {            /* remaining prime factor > sqrt(n) */
        pr[*cnt] = rem;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined, but return 0 */
    init_irreducibility();
    init_primes();

    if (n == 1) return (seq_t)0;     /* no proper divisors */

    /* factor n */
    uint64_t fac_pr[10];
    uint64_t fac_ex[10];
    size_t fac_cnt = 0;
    factor_uint64(n, fac_pr, fac_ex, &fac_cnt);

    /* generate all divisors */
    uint32_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = fac_pr[i];
        uint64_t e = fac_ex[i];
        size_t old_cnt = div_cnt;
        uint64_t p_pow = 1;
        for (uint64_t k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < old_cnt; ++j)
                divs[div_cnt++] = (uint32_t)(divs[j] * p_pow);
        }
    }

    /* count proper divisors that are reducible polynomials */
    unsigned long long result = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint32_t d = divs[i];
        if (d == n) continue;        /* proper divisor only */
        if (!irreducible[d]) result++;
    }

    return (seq_t)result;
}
