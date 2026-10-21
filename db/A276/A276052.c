/*
 * A276052  Least k > 1 such that phi(k*n-1) = phi(k*n+1), or -1 if no such k
 *          exists.
 * https://oeis.org/A276052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A276052: Least k > 1 such that φ(k*n-1) = φ(k*n+1).
 * If no such k exists the function returns (seq_t)-1.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/phi.h"

/* Tables holding φ(m) for 0 ≤ m ≤ cur_limit+1 and a mask indicating
 * whether φ(m‑1) = φ(m+1).  They are extended by build_sieve(). */
static uint32_t *phi_tbl = NULL;
static uint8_t  *good_tbl = NULL;
static idx_t     cur_limit = 0;

/* Extend the sieve tables up to 'limit'.  The function is idempotent
 * when called with a limit ≤ cur_limit. */
static void build_sieve(idx_t limit)
{
    if (limit <= cur_limit)
        return;

    idx_t sz = limit + 2;                  /* indices 0 … limit+1 */
    uint32_t *new_phi  = (uint32_t *)calloc((size_t)sz, sizeof(uint32_t));
    uint8_t  *new_good = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));

    if (!new_phi || !new_good)
        abort();

    /* initialise φ[i] = i */
    for (idx_t i = 0; i < sz; ++i)
        new_phi[i] = (uint32_t)i;

    /* linear sieve for Euler's totient */
    for (idx_t p = 2; p <= limit + 1; ++p) {
        if (new_phi[p] == (uint32_t)p) {                  /* p is prime */
            for (idx_t j = p; j <= limit + 1; j += p) {
                new_phi[j] = (uint32_t)((new_phi[j] / (uint32_t)p) *
                                        (uint32_t)(p - 1));
            }
        }
    }

    /* predicate table: φ(m‑1) = φ(m+1) ? */
    new_good[0] = new_good[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        new_good[i] = (new_phi[i - 1] == new_phi[i + 1]) ? 1 : 0;

    free(phi_tbl);
    free(good_tbl);
    phi_tbl = new_phi;
    good_tbl = new_good;
    cur_limit = limit;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                    /* offset 1; undefined for n==0 */
        return 0;

    static const idx_t INITIAL_LIMIT = 1000000;
    build_sieve(INITIAL_LIMIT);

    for (seq_t k = 2;; ++k) {      /* search for the least k>1 */
        seq_t m = (seq_t)n * k;

        if (m <= (seq_t)cur_limit) {
            if (good_tbl[(size_t)m])
                return k;
        } else {
            /* fallback uses the provided phi routine */
            seq_t phi1 = phi((idx_t)(m - 1));
            seq_t phi2 = phi((idx_t)(m + 1));
            if (phi1 == phi2)
                return k;
        }
    }
    /* unreachable, but satisfies the return type */
    return (seq_t)-1;
}
