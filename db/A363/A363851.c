/*
 * A363851  Number of divisors of 7*n-4 of form 7*k+1.
 * https://oeis.org/A363851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of (7*n-4) such that d ≡ 1 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute M = 7*n - 4 */
    uint64_t m = 7ULL * n - 4ULL;

    /* simple trial division factorisation of m */
    uint64_t pf[20];
    int    exp[20];
    int    pcnt = 0;

    uint64_t tmp = m;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            pf[pcnt]  = p;
            exp[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1)
    {
        pf[pcnt]  = tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* DP over prime factors: cur[res] = #ways to obtain residue res (mod 7) */
    seq_t cur[7] = {0};
    cur[1] = 1;   /* the empty product contributes residue 1 */

    for (int i = 0; i < pcnt; ++i)
    {
        uint64_t p = pf[i];
        int      emax = exp[i];

        /* residues of p^e (mod 7) for e = 0..emax */
        unsigned char resid[65];
        resid[0] = 1 % 7;
        unsigned char pmod7 = p % 7;
        for (int e = 1; e <= emax; ++e)
        {
            resid[e] = (resid[e - 1] * pmod7) % 7;
        }

        seq_t nxt[7] = {0};

        for (int r = 0; r < 7; ++r)
        {
            if (cur[r] == 0) continue;
            for (int e = 0; e <= emax; ++e)
            {
                int nr = (r * resid[e]) % 7;
                nxt[nr] += cur[r];
            }
        }
        /* copy nxt back to cur */
        for (int r = 0; r < 7; ++r) cur[r] = nxt[r];
    }

    return cur[1];   /* count of divisors ≡ 1 (mod 7) */
}
