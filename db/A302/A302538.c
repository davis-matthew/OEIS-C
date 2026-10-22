/*
 * A302538  Number of partitions of n into two parts (p,q) with p <= q such
 *          that p is squarefree and q is semiprime.
 * https://oeis.org/A302538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} μ(i)^2 * [Ω(n-i) = 2]
 * Range:     u8 n <= 2051, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* enumerates partitions n = p + q with p <= q, p square‑free, q a semiprime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* smallest‑prime‑factor table, up to the largest required n‑1 */
static idx_t *spf = NULL;
static size_t spf_limit = 0;

/* build or extend the SPF table up to `limit` (inclusive) */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    idx_t *new_spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!new_spf) return;          /* failure – caller will return 0 */

    for (idx_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    /* linear sieve for SPF */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = (size_t)limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                         /* offset starts at 1 */

    /* we need SPF for all numbers up to n‑1 (for Omega(n‑i)) */
    idx_t needed = (n > 0) ? n - 1 : 0;
    build_spf(needed);

    seq_t result = 0;
    idx_t limit = n >> 1;                        /* floor(n/2) */

    for (idx_t i = 1; i <= limit; ++i) {
        /* check if i is square‑free: μ(i)^2 = 1 */
        bool squarefree = true;
        idx_t x = i;
        while (x > 1 && squarefree) {
            idx_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
                if (cnt > 1) {                  /* repeated prime factor */
                    squarefree = false;
                    break;
                }
            }
        }
        if (!squarefree) continue;

        /* count Ω(n-i) */
        idx_t y = n - i;
        int omega = 0;
        x = y;
        while (x > 1) {
            idx_t p = spf[x];
            while (x % p == 0) {
                x /= p;
                ++omega;
            }
        }

        if (omega == 2) ++result;                /* q is a semiprime */
    }

    return result;
}
