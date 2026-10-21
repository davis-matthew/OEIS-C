/*
 * A273088  a(n) is a multiple of 6 and a(n)-1 or a(n)+1 is an isolated
 *          (non-twin) prime number.
 * https://oeis.org/A273088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 4822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: multiples of 6 where m‑1 or m+1 is an isolated prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n ≥ 1 */

    /* state for memoisation */
    static seq_t nextCandidate = (seq_t)24;   /* first multiple of 6 to test */

#ifndef __cplusplus
    static seq_t *cacheC = NULL;
    static size_t szC = 0;
    static size_t capC = 0;
#else
    static std::vector<seq_t> cacheV;
#endif

    /* generate terms until we have at least n of them */
    while (
#ifndef __cplusplus
           szC < n
#else
           cacheV.size() < n
#endif
    ) {
        seq_t m = nextCandidate;
        nextCandidate += 6;          /* next multiple of 6 */

        bool good = false;

        /* check m‑1 */
        seq_t p = m - 1;
        if (isPrimeGeneric(p)) {
            if (!isPrimeGeneric(p - 2) && !isPrimeGeneric(p + 2))
                good = true;
        }

        /* if not good yet, check m+1 */
        if (!good) {
            p = m + 1;
            if (isPrimeGeneric(p)) {
                if (!isPrimeGeneric(p - 2) && !isPrimeGeneric(p + 2))
                    good = true;
            }
        }

        if (good) {
#ifdef __cplusplus
            cacheV.push_back(m);
#else
            if (szC == capC) {
                size_t new_cap = capC == 0 ? 16 : capC * 2;
                seq_t *tmp = (seq_t *)realloc(cacheC, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cacheC = tmp;
                capC = new_cap;
            }
            cacheC[szC++] = m;
#endif
        }
    }

    /* return the n‑th term (index 1‑based) */
#ifdef __cplusplus
    return cacheV[n - 1];
#else
    return cacheC[n - 1];
#endif
}
