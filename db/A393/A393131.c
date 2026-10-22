/*
 * A393131  a(n) = EulerPhi(2*n+1)/A329726(n); index of witnesses for
 *          Solovay-Strassen primality test of 2*n+1 as a subgroup of
 *          (Z/(2*n+1)Z)*.
 * https://oeis.org/A393131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = EulerPhi(2*n+1)/A329726(n) where A329726(n) is the number
 *            of bases a modulo 2*n+1 such that a^n ≡ Jacobi(a,2*n+1) (mod
 *            2*n+1). This implementation follows the OEIS definition and
 *            computes the value using factorisation of 2*n+1. The routine is
 *            correct for all n ≥ 1
 * Range:     u8 n <= 289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Greatest common divisor for 64‑bit integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Count the exponent of 2 in an unsigned 64‑bit integer */
static int v2_uint64(uint64_t x)
{
    int c = 0;
    while ((x & 1ULL) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Trial division factorisation for numbers up to a few millions.
   The result is stored in arrays pr (primes) and ex (exponents),
   and *cnt returns the number of distinct prime factors. */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    /* k = 2*n+1, often called the composite modulus */
    uint64_t k = 2ULL * (uint64_t)n + 1ULL;
    uint64_t km1 = k - 1;

    /* Pre‑compute v2(k-1) once */
    int v2_km = v2_uint64(km1);

    /* Factor k */
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint64(k, primes, exps, &cnt);

    /* Variables for the 2‑adic valuation of a(n) */
    int sum_R = 0;
    bool S_nonempty = false;
    bool T_nonempty = false;

    /* 2‑adic valuations of φ(k) and of product of gcd(p-1,k-1) */
    int v2_phi = 0;
    int v2_gcd_prod = 0;

    /* φ(k) and product of gcd(p-1,k-1) */
    seq_t phi = 1;
    seq_t prod_gcd = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];

        /* v2(p-1) */
        int vp = v2_uint64(p - 1);

        /* Update φ(k) */
        v2_phi += vp;                       /* 2‑adic part from p-1 */
        seq_t tp = 1;
        for (int j = 0; j < e - 1; ++j)       /* p^(e-1) */
            tp *= (seq_t)p;
        phi *= tp * (seq_t)(p - 1);

        /* Update product of gcd(p-1,k-1) */
        uint64_t g64 = gcd_u64(p - 1, km1);
        v2_gcd_prod += v2_uint64(g64);
        prod_gcd *= (seq_t)g64;

        /* Classify the prime into R, S or T */
        if (v2_km <= vp) {
            /* R set */
            sum_R += vp - v2_km + 1;
        } else {
            /* v2_km > vp */
            if (e & 1)
                S_nonempty = true;
            else
                T_nonempty = true;
        }
    }

    /* Determine l according to the definition */
    int l_val;
    if (S_nonempty)
        l_val = 1;
    else if (T_nonempty)
        l_val = 0;
    else
        l_val = -1;

    /* Final 2‑adic valuation of a(n) */
    int v2_a = sum_R + l_val;
    if (v2_a < 0) v2_a = 0;  /* safety, should not happen */

    /* Remove 2-powers from phi and prod_gcd to get odd parts */
    seq_t odd_phi = phi >> (unsigned)v2_phi;
    seq_t odd_gcd = prod_gcd >> (unsigned)v2_gcd_prod;

    /* Find the odd part of a(n) = odd_phi / odd_gcd (exact division) */
    seq_t odd_ratio = odd_phi / odd_gcd;

    /* Final result: multiply by the remaining 2‑power */
    seq_t result = odd_ratio << (unsigned)v2_a;
    return result;
}
