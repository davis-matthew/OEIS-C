/*
 * A384644  a(n) is the least k such that {A007504(1), A007504(2), ...,
 *          A007504(k)} mod n = {0,...,n-1}.
 * https://oeis.org/A384644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimal k such that the set of cumulative sums * of
 *            the first k primes taken modulo n contains every residue 0..n-1.
 *            * The function searches for that k by generating primes on
 *            demand
 * Range:     u8 n <= 5, u16 n <= 850, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External types supplied by the build system */
seq_t a(idx_t n)
{
    /* trivial cases ----------------------------------------------------- */
    if (n <= 1) return 1;           /* a(1) = 1 */
    if (n == 2) return 2;           /* a(2) = 2 */

    /* static sieve of primes up to LIMIT -------------------------------- */
    static uint32_t *primes     = NULL;   /* array of primes            */
    static size_t   primes_len  = 0;      /* number of primes stored    */
    static size_t   primes_cap  = 0;      /* allocated capacity         */
    static unsigned char *is_comp = NULL;/* composite flags             */

    /* sieve limit: 20 000 000 → ~1 270 000 primes, ample for n ≤ 10 000   */
    static const size_t LIMIT = 20000000U;

    /* build sieve on first call ----------------------------------------- */
    if (primes == NULL) {
        is_comp = (unsigned char*)calloc(LIMIT + 1, sizeof(unsigned char));
        if (!is_comp) return 0;            /* out of memory */

        primes_cap = LIMIT / 10;           /* initial capacity */
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) { free(is_comp); return 0; }

        /* Sieve of Eratosthenes */
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (!is_comp[i]) {
                /* store prime */
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(is_comp); free(primes); return 0; }
                    primes = tmp;
                }
                primes[primes_len++] = (uint32_t)i;

                /* mark composites starting from i*i */
                if ((uint64_t)i * i <= LIMIT) {
                    for (size_t j = i * i; j <= LIMIT; j += i)
                        is_comp[j] = 1;
                }
            }
        }
        free(is_comp);  /* no longer needed */
        is_comp = NULL;
    }

    /* find minimal k ----------------------------------------------------- */
    unsigned char *seen = (unsigned char*)calloc((size_t)n, sizeof(unsigned char));
    if (!seen) return 0;           /* out of memory */

    size_t seen_cnt = 0;          /* distinct residues */
    seq_t  sum_mod  = 0;          /* running sum modulo n */
    size_t k = 0;                 /* prime index (0‑based) */

    for (; k < primes_len; ++k) {
        seq_t p = (seq_t)primes[k];
        sum_mod += p;
        sum_mod %= (seq_t)n;      /* keep in range [0, n‑1] */

        idx_t idx = (idx_t)sum_mod;
        if (!seen[idx]) {
            seen[idx] = 1;
            ++seen_cnt;
            if (seen_cnt == n) break;  /* all residues seen */
        }
    }

    free(seen);

    /* k is zero‑based; the required count is k+1 */
    return (seq_t)(k + 1);
}
