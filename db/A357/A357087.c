/*
 * A357087  E.g.f. satisfies A(x) * log(A(x)) = (exp(x*A(x)) - 1)^3.
 * https://oeis.org/A357087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A357087:
   a(n) = Sum_{k=0..⌊n/3⌋} (3k)! * (n-k+1)^(k-1) * Stirling2(n,3k)/k!
   Using 64‑bit or 128‑bit arithmetic for intermediate values to avoid
   overflow when the final term fits in seq_t. */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static idx_t max_pre = (idx_t)-1;        /* last precomputed n */
    static seq_t *fact = NULL;              /* factorial[0..max_pre]   */
    static seq_t *a_cache = NULL;           /* a[n] values             */
    static seq_t *Sprev = NULL;             /* previous row of Stirling*/
    static seq_t *Scurr = NULL;             /* current  row            */

    /* handle first call */
    if (max_pre == (idx_t)-1) {
        max_pre = 0;
        fact = (seq_t *)malloc(sizeof(seq_t) * 1);
        a_cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        Sprev = (seq_t *)malloc(sizeof(seq_t) * 1);
        Scurr = (seq_t *)malloc(sizeof(seq_t) * 1);

        fact[0] = (seq_t)1;
        a_cache[0] = (seq_t)1;        /* a(0)=1 */
        Sprev[0] = (seq_t)1;          /* S(0,0)=1 */

        return a_cache[0];
    }

    /* return cached value if already computed */
    if (n <= max_pre)
        return a_cache[n];

    /* extend precomputation up to n */
    {
        size_t old_size = (size_t)(max_pre + 1);

        /* factorial array expansion */
        fact = (seq_t *)realloc(fact, sizeof(seq_t) * (n + 1));
        for (idx_t i = max_pre + 1; i <= n; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        /* a_cache expansion */
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * (n + 1));

        /* Stirling arrays expansion */
        Sprev = (seq_t *)realloc(Sprev, sizeof(seq_t) * (n + 1));
        Scurr = (seq_t *)realloc(Scurr, sizeof(seq_t) * (n + 1));

        /* compute rows from max_pre+1 to n */
        for (idx_t cur = max_pre + 1; cur <= n; ++cur) {
            /* ensure Sprev[cur] == 0 (S(cur-1,cur)=0) */
            Sprev[cur] = (seq_t)0;

            /* compute Stirling row Scurr[0..cur] */
            Scurr[0] = (seq_t)0;
            for (idx_t m = 1; m <= cur; ++m) {
                seq_t term1 = Sprev[m - 1];
                seq_t term2 = (seq_t)m * Sprev[m];
                Scurr[m] = term1 + term2;
            }

            /* compute a[cur] using the closed form */
            unsigned __int128 sum128 = 0;
            idx_t k_max = cur / 3;
            for (idx_t k = 0; k <= k_max; ++k) {
                idx_t m = 3 * k;
                seq_t stir = Scurr[m];
                if (stir == (seq_t)0) continue;

                seq_t fact_3k = fact[3 * k];
                seq_t fact_k  = fact[k];
                seq_t base = (seq_t)(cur - k + 1);

                seq_t power;
                if (k == 0)
                    power = (seq_t)1;
                else
                    power = big_pow(base, (seq_t)(k - 1));

                /* use 128‑bit arithmetic for the product */
                unsigned __int128 prod = (unsigned __int128)fact_3k;
                prod *= (unsigned __int128)power;
                prod *= (unsigned __int128)stir;
                prod /= (unsigned __int128)fact_k;

                sum128 += prod;
            }
            a_cache[cur] = (seq_t)sum128;

            /* swap Sprev and Scurr for next iteration */
            seq_t *tmp = Sprev;
            Sprev = Scurr;
            Scurr = tmp;
        }
        max_pre = n;
    }

    return a_cache[n];
}
