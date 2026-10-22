/*
 * A394638  Start of exactly four even numbers that are totient numbers
 *          (A002202).
 * https://oeis.org/A394638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the starting value of a run of exactly four consecutive
 *            even totient numbers (entries of A002202)
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


seq_t a(idx_t n) {
    /* static state -------------------------------------------------------- */
    static idx_t *runs = NULL;      /* array of run starts */
    static size_t runs_cnt = 0;     /* number of runs collected */

    if (n == 0) return 0; /* undefined for n < 1 */

    /* ensure we have at least n runs pre‑computed */
    if (runs_cnt < (size_t)n) {
        idx_t limit = 100000;          /* initial sieve size */

        while (1) {
            size_t sz = (size_t)limit + 1;
            uint32_t *phi = (uint32_t *)calloc(sz, sizeof(uint32_t));
            if (!phi) return 0;        /* allocation error */

            /* initialise phi */
            for (size_t i = 0; i < sz; ++i) phi[i] = (uint32_t)i;

            /* sieve for phi */
            for (size_t i = 2; i < sz; ++i) {
                if (phi[i] == (uint32_t)i) {        /* i is prime */
                    for (size_t j = i; j < sz; j += i) {
                        phi[j] = (phi[j] / (uint32_t)i) * (uint32_t)(i - 1);
                    }
                }
            }

            /* mark even totient values */
            bool *even_phi = (bool *)calloc(sz, sizeof(bool));
            if (!even_phi) {
                free(phi);
                return 0;
            }
            for (size_t i = 2; i < sz; ++i) {
                uint32_t v = phi[i];
                if ((v & 1U) == 0 && v <= limit) {
                    even_phi[v] = true;
                }
            }

            /* collect runs */
            size_t cap = 16;
            idx_t *tmp = (idx_t *)malloc(cap * sizeof(idx_t));
            if (!tmp) {
                free(phi);
                free(even_phi);
                return 0;
            }
            size_t count = 0;

            for (idx_t m = 2; m + 8 <= limit; m += 2) {
                if (even_phi[(size_t)m] && even_phi[(size_t)(m + 2)] &&
                    even_phi[(size_t)(m + 4)] && even_phi[(size_t)(m + 6)] &&
                    !even_phi[(size_t)(m - 2)] && !even_phi[(size_t)(m + 8)]) {

                    if (count == cap) {
                        cap <<= 1;
                        idx_t *np = (idx_t *)realloc(tmp, cap * sizeof(idx_t));
                        if (!np) {
                            free(tmp);
                            free(phi);
                            free(even_phi);
                            return 0;
                        }
                        tmp = np;
                    }
                    tmp[count++] = m;
                }
            }

            free(phi);
            free(even_phi);

            /* replace previous runs (if any) */
            free(runs);
            runs = tmp;
            runs_cnt = count;

            if (runs_cnt >= (size_t)n) {
                break;                 /* enough runs found */
            }

            /* try a larger limit next time */
            limit <<= 1;
        }
    }

    return (seq_t)runs[n - 1];
}
