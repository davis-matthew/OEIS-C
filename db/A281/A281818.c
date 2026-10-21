/*
 * A281818  a(n) is the least number k such that sigma(n) + sigma(k) =
 *          sigma(abs(n-k)), or -1 if such a number does not exist.
 * https://oeis.org/A281818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   a(n) = least positive k such that sigma(n)+sigma(k) =
 *            sigma(|n-k|), * or -1 if no such k exists. Uses a sieve of sigma
 *            values up to a fixed * limit and checks all candidate absolute
 *            differences d=|n-k|
 * Range:     i8 n <= 1, i16 n <= 7, i32 n <= 7, i64 n <= 7, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *sigma_tbl = NULL;
static const idx_t SIGMA_LIMIT = 10000000;   /* 10^7 entries */

static void init_sigma(void)
{
    if (sigma_tbl != NULL)
        return;
    sigma_tbl = (uint32_t *)calloc(SIGMA_LIMIT + 1, sizeof(uint32_t));
    if (!sigma_tbl)
        return; /* malloc failure fallback, will return -1 for all */
    for (idx_t i = 1; i <= SIGMA_LIMIT; ++i) {
        for (idx_t j = i; j <= SIGMA_LIMIT; j += i)
            sigma_tbl[j] += (uint32_t)i;
    }
}

static inline uint32_t sigma_at(idx_t n)
{
    if (n <= SIGMA_LIMIT)
        return sigma_tbl[n];
    /* fallback: compute sigma via trial factorization (slow) */
    uint32_t sum = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t term = 1;
            uint64_t pk = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pk *= p;
                term += pk;
            }
            sum *= (uint32_t)term;
        }
    }
    if (tmp > 1) sum *= (uint32_t)(1 + tmp);
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-1; /* offset mismatch */
    init_sigma();
    uint32_t sigma_n = sigma_at(n);
    idx_t best = (idx_t)-1; /* sentinel for not found */

    for (idx_t d = 1; d <= SIGMA_LIMIT; ++d) {
        uint32_t sigma_d = sigma_tbl[d];
        if (sigma_d <= sigma_n) continue; /* target would be non‑positive */
        uint32_t target = sigma_d - sigma_n;

        idx_t k1 = (n > d) ? (n - d) : 0;
        if (k1 > 0 && k1 <= SIGMA_LIMIT && sigma_tbl[k1] == target) {
            if (best == (idx_t)-1 || k1 < best) best = k1;
        }
        idx_t k2 = n + d;
        if (k2 <= SIGMA_LIMIT && sigma_tbl[k2] == target) {
            if (best == (idx_t)-1 || k2 < best) best = k2;
        }
        /* early exit: if we found k=1 it's the minimal possible */
        if (best == 1) break;
    }

    if (best == (idx_t)-1)
        return (seq_t)-1;
    return (seq_t)best;
}
