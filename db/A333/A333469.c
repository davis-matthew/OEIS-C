/*
 * A333469  Number of integers in base n having exactly four distinct digits
 *          such that the number formed by the consecutive subsequence of the
 *          initial j digits is divisible by j for all j in {1,2,3,4}.
 * https://oeis.org/A333469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 568, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count 4‑digit base‑n numbers with distinct digits
   whose prefixes of length j are divisible by j (j=1…4).
   For each base n≥4, iterate over digit residues modulo 12,
   check the modular conditions (mod 2, 3, 4) and count using
   the multiplicities of digits with each residue. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 0: for bases <4 no 4‑digit numbers exist */
    if (n < 4) return 0;

    unsigned long long total = 0ULL;

    /* Residues of n modulo 2,3,4 and powers needed */
    int n_mod2 = n & 1;
    int n_mod3 = n % 3;
    int n_mod4 = n % 4;
    int n_sqr3 = (n_mod3 * n_mod3) % 3;
    int n_pow2 = (n_mod4 * n_mod4) % 4;
    int n_pow3 = (n_pow2 * n_mod4) % 4;

    /* Pre‑compute how many digits of each residue 0…11 occur
       in the set {0,…,n‑1}. */
    unsigned int cnt[12];
    for (int r = 0; r < 12; ++r)
    {
        if (n <= (idx_t)r) cnt[r] = 0;
        else
            cnt[r] = (unsigned)((n - 1 - (idx_t)r) / 12 + 1);
    }

    /* Enumerate residues for the 4 digits (a,b,c,d) */
    for (int ra = 0; ra < 12; ++ra)
    {
        if (cnt[ra] == 0) continue;
        int a_choices = (ra == 0 ? (int)cnt[ra] - 1 : (int)cnt[ra]);
        if (a_choices <= 0) continue;  /* no non‑zero a available */

        int ra_mod2 = ra & 1;
        int ra_mod3 = ra % 3;
        int ra_mod4 = ra % 4;

        for (int rb = 0; rb < 12; ++rb)
        {
            if (cnt[rb] == 0) continue;

            /* Mod 2 condition: a*n + b ≡ 0 (mod 2) */
            if (((ra_mod2 * n_mod2 + (rb & 1)) & 1) != 0) continue;

            int rb_choices = (int)cnt[rb] - (ra == rb ? 1 : 0);
            if (rb_choices <= 0) continue;

            int rb_mod3 = rb % 3;
            int rb_mod4 = rb % 4;

            for (int rc = 0; rc < 12; ++rc)
            {
                if (cnt[rc] == 0) continue;

                /* Mod 3 condition: a*n² + b*n + c ≡ 0 (mod 3) */
                if (((ra_mod3 * n_sqr3 + rb_mod3 * n_mod3 + (rc % 3)) % 3) != 0)
                    continue;

                int rc_choices = (int)cnt[rc]
                                 - (ra == rc ? 1 : 0)
                                 - (rb == rc ? 1 : 0);
                if (rc_choices <= 0) continue;

                int rc_mod4 = rc % 4;

                for (int rd = 0; rd < 12; ++rd)
                {
                    if (cnt[rd] == 0) continue;

                    /* Mod 4 condition: a*n³ + b*n² + c*n + d ≡ 0 (mod 4) */
                    if ((((ra_mod4 * n_pow3)
                           + (rb_mod4 * n_pow2)
                           + (rc_mod4 * n_mod4)
                           + rd) & 3) != 0)
                        continue;

                    int rd_choices = (int)cnt[rd]
                                     - (ra == rd ? 1 : 0)
                                     - (rb == rd ? 1 : 0)
                                     - (rc == rd ? 1 : 0);
                    if (rd_choices <= 0) continue;

                    unsigned long long ways =
                        (unsigned long long)a_choices *
                        (unsigned long long)rb_choices *
                        (unsigned long long)rc_choices *
                        (unsigned long long)rd_choices;

                    total += ways;
                }
            }
        }
    }

    return (seq_t)total;
}
