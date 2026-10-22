/*
 * A363856  Number of divisors of 7*n-4 of form 7*k+6.
 * https://oeis.org/A363856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of divisors d of (7*n-4) such that d ≡ 6 (mod 7)
 * (equivalently d ≡ 4 (mod 7)).  The function factors (7*n-4) and
 * counts the admissible divisors using a dynamic programming over residues.
 */

#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* Compute m = 7*n - 4 using a 64‑bit integer to avoid overflow. */
    idx_t m_tmp = 7 * n - 4;               /* m fits in idx_t up to n≈10000 */
    if (m_tmp == 0) return (seq_t)0;        /* safety, though never hit */

    /* Factor m_tmp into prime powers.  Since m_tmp ≤ 69996 for n≤10000,
       at most 10 distinct prime factors exist.  We store them in two
       small arrays. */
    idx_t primes[15];
    idx_t exps[15];
    int pf_cnt = 0;

    idx_t m = m_tmp;
    /* factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        primes[pf_cnt] = 2;
        exps[pf_cnt]   = (idx_t)e;
        ++pf_cnt;
    }
    /* trial division by odd numbers up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]   = (idx_t)e;
            ++pf_cnt;
        }
    }
    /* If remaining m is a prime > sqrt(original m) */
    if (m > 1) {
        primes[pf_cnt] = m;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* Dynamic programming over residues modulo 7.
     * count[r]  – number of divisor combinations producing residue r modulo 7.
     */
    seq_t count[7] = {0};
    count[1 % 7] = 1; /* the divisor 1 */

    for (int i = 0; i < pf_cnt; ++i) {
        idx_t p = primes[i];
        int   e = (int)exps[i];

        /* precompute p^k mod 7 for k = 0..e */
        int mod7 = (int)(p % 7);
        int pow_mod[20];       /* e ≤ 15 for m ≤ 69996 */
        pow_mod[0] = 1 % 7;
        for (int k = 1; k <= e; ++k)
            pow_mod[k] = (pow_mod[k - 1] * mod7) % 7;

        seq_t next[7] = {0};

        for (int r = 0; r < 7; ++r) {
            if (count[r] == 0) continue;
            for (int k = 0; k <= e; ++k) {
                int new_r = (r * pow_mod[k]) % 7;
                next[new_r] += count[r];
            }
        }

        /* copy back */
        for (int r = 0; r < 7; ++r) count[r] = next[r];
    }

    return (seq_t)count[6];   /* number of divisors ≡ 6 (mod 7) */
}
