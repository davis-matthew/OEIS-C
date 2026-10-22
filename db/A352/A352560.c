/*
 * A352560  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 37.
 * https://oeis.org/A352560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} f(p,e) where • If (37|p)=0 (p=37, ramified)
 *            f(p,e)=P(e) (partitions of e) • If (37|p)=1 (p split)
 *            f(p,e)=∑_{k=0}^{e} P(k)·P(e−k) • If (37|p)=−1 (p inert) and e
 *            even f(p,e)=P(e/2) • Otherwise f(p,e)=0 (no module of that size)
 *            P(k) denotes the integer partition function
 * Range:     u8 n <= 6702, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*-----------------------------------------------------------------------
   Partition numbers P(0)…P(60)  (corrected from the earlier wrong code)
  -----------------------------------------------------------------------*/
static seq_t  partitions[61];
static bool   partitions_initialized = false;

static void init_partitions(void)
{
    if (partitions_initialized)
        return;
    /* P(0)=1, P(n)=0 for n<0 */
    for (int i = 0; i <= 60; ++i)
        partitions[i] = 0;
    partitions[0] = 1;
    /* ordinary partition DP: add each coin from 1..60 once */
    for (int coin = 1; coin <= 60; ++coin) {
        for (int s = coin; s <= 60; ++s) {
            partitions[s] += partitions[s - coin];
        }
    }
    partitions_initialized = true;
}

/*-----------------------------------------------------------------------
   Modular exponentiation for 64‑bit values, used in Euler's criterion
-----------------------------------------------------------------------*/
static seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1)
        return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

/*-----------------------------------------------------------------------
   Legendre symbol (37|p) for odd prime p (returns 0, +1, or –1)
-----------------------------------------------------------------------*/
static int legendre_37(uint64_t p)
{
    if (p == 37)          /* ramified */
        return 0;
    if (p == 2)           /* 37 ≡ 5 (mod 8) → inert */
        return -1;
    /* Euler: (37|p) = 37^{(p−1)/2} mod p */
    seq_t L = mod_pow_uint64(37, (p - 1) >> 1, p);
    if (L == 1)
        return 1;        /* split */
    if (L == p - 1)
        return -1;       /* inert */
    return 0;            /* should not occur for odd prime */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* undefined for offset 1 but harmless */

    init_partitions();

    seq_t result = 1;
    uint64_t x = (uint64_t)n;

    for (uint64_t p = 2; p <= x / p; ++p) {
        if (x % p != 0)
            continue;

        /* exponent e of prime p in n */
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }

        int leg = legendre_37(p);
        seq_t local = 0;

        if (leg == 0) {                      /* ramified (p=37) */
            local = partitions[e];
        } else if (leg == 1) {               /* split prime */
            seq_t sum = 0;
            for (int k = 0; k <= e; ++k)
                sum += partitions[k] * partitions[e - k];
            local = sum;
        } else { /* inert prime */
            if (e & 1)                       /* odd exponent impossible */
                return 0;
            local = partitions[e >> 1];
        }

        if (local == 0)
            return 0;
        result *= local;
    }

    /* Remaining factor: a single prime > sqrt(original n) */
    if (x > 1) {
        int e = 1;                           /* exponent is 1 */
        int leg = legendre_37(x);
        seq_t local = 0;

        if (leg == 0) {                      /* ramified */
            local = partitions[e];
        } else if (leg == 1) {               /* split prime, e = 1 */
            local = partitions[0] * partitions[1] +
                     partitions[1] * partitions[0];  /* 2 */
        } else {                             /* inert, e = 1 → odd → 0 */
            if (e & 1)
                return 0;
            local = partitions[e >> 1];
        }

        if (local == 0)
            return 0;
        result *= local;
    }

    return result;
}
