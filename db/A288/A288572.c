/*
 * A288572  a(n) = smallest positive integer k such that (n^3)^k == 1 mod
 *          (n+1)^3.
 * https://oeis.org/A288572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 2253, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A288572
 *  a(n) = smallest positive integer k such that (n^3)^k ≡ 1  (mod (n+1)^3)
 *  (a(n) is the multiplicative order of n^3 modulo (n+1)^3)
 *
 *  The implementation uses a standard multiplicative‑order routine:
 *  1. Compute m = (n+1)^3 and a = n^3 mod m.
 *  2. Compute φ(m) = (n+1)^2 φ(n+1).
 *  3. Generate the set of distinct primes that divide φ(m)
 *     (those dividing n+1 and those dividing p−1 for each prime p|n+1).
 *  4. Starting from order = φ(m) reduce it by dividing out each prime factor
 *     whenever a^order/p ≡ 1 (mod m).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*=== helper: modular multiplication without overflow ======================================*/
static seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    seq_t result = 0;
    a %= mod;
    while (b > 0) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a + a) % mod;
        b >>= 1;
    }
    return result;
}

/*=== helper: modular exponentiation (using mul_mod_seq) ==============================*/
static seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = mul_mod_seq(result, base, mod);
        }
        base = mul_mod_seq(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/*=== helper: trial division factorisation of a 32‑bit number ======================*/
static void factor_uint32(uint32_t n, uint32_t *factors, int *exps, size_t *cnt)
{
    *cnt = 0;
    uint32_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            int e = 0;
            do {
                n /= d;
                ++e;
            } while (n % d == 0);
            factors[(*cnt)] = d;
            exps[(*cnt)]   = e;
            ++(*cnt);
        }
        d = (d == 2) ? 3 : d + 2;
    }
    if (n > 1) {
        factors[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        ++(*cnt);
    }
}

/*=== helper: insert a prime into a distinct list ======================================*/
static void add_prime(uint32_t *list, size_t *cnt, uint32_t p)
{
    for (size_t i = 0; i < *cnt; ++i) {
        if (list[i] == p)
            return;
    }
    list[(*cnt)++] = p;
}

/*=== main sequence function =========================================================*/
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is defined for n≥1; 0 is unused */
        return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t mp1   = n_seq + 1;                  /* n+1 */
    seq_t mod   = mp1 * mp1 * mp1;            /* (n+1)^3, fits in seq_t for n≤10000 */

    /* base = n^3 mod m */
    seq_t base = mod_pow_seq(n_seq, 3, mod);

    /* factorisation of n+1 */
    uint32_t p_list[32];
    int      p_exp[32];
    size_t   pcnt = 0;
    factor_uint32((uint32_t)mp1, p_list, p_exp, &pcnt);

    /* φ(n+1) using the standard formula φ(p^e) = p^e - p^(e-1) */
    seq_t phi_n = mp1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = p_list[i];
        phi_n = phi_n / (seq_t)p * (seq_t)(p - 1);
    }

    /* φ((n+1)^3) = (n+1)^2 * φ(n+1) */
    seq_t phi_m = phi_n * mp1 * mp1;

    /* Build the set of distinct primes dividing φ(m) */
    uint32_t primes_order[128];
    size_t   po_cnt = 0;

    /* primes dividing n+1 */
    for (size_t i = 0; i < pcnt; ++i)
        add_prime(primes_order, &po_cnt, p_list[i]);

    /* primes dividing p-1 for each prime factor p of n+1 */
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t q = p_list[i] - 1;
        if (q > 1) {
            uint32_t fp[32];
            int      fe[32];
            size_t   fcnt = 0;
            factor_uint32(q, fp, fe, &fcnt);
            for (size_t j = 0; j < fcnt; ++j)
                add_prime(primes_order, &po_cnt, fp[j]);
        }
    }

    /* Reduce the order starting from φ(m) */
    seq_t order = phi_m;
    for (size_t i = 0; i < po_cnt; ++i) {
        uint32_t p = primes_order[i];
        while (order % (seq_t)p == 0) {
            seq_t cand = order / (seq_t)p;
            if (mod_pow_seq(base, (idx_t)cand, mod) == (seq_t)1)
                order = cand;
            else
                break;
        }
    }

    return order;
}
