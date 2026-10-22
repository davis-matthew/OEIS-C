/*
 * A375464  a(n) is the number of nonnegative numbers k < n such that the
 *          equation (n-k)*x - k = y^2 is solvable.
 * https://oeis.org/A375464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{1 <= j <= n : (-n mod j) is a quadratic residue modulo
 *            j}
 * Range:     u8 n <= 515, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_qr_mod_2pow, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h"   /* provides: int jacobi_symbol(uint64_t a, uint64_t n); */
#include "../utils/is_qr_mod_2pow.h"

static uint64_t *spf = NULL;          /* smallest prime factor array */
static idx_t spf_limit = 0;           /* size of the array (last index filled) */

/* Builds SPF up to 'limit' (inclusive).  Re‑allocates if necessary. */
static void build_spf(idx_t limit)
{
    if (limit <= spf_limit && spf != NULL) return;

    if (spf != NULL) free(spf);

    spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (spf == NULL) abort();          /* out of memory */

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Main function required by the template.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* nothing defined below the offset */

    /* ensure SPF table covers n */
    build_spf(n);

    seq_t count = 0;

    for (idx_t j = 1; j <= n; ++j) {
        bool square_residue = true;
        idx_t t = j;

        while (t > 1) {
            uint64_t p = spf[t];
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }

            /* compute -n  modulo  p^e */
            idx_t mod = 1;
            for (idx_t k = 0; k < e; ++k) mod *= p;   /* p^e (fits in idx_t for j ≤ n) */
            idx_t a_mod = (mod - (n % mod)) % mod;    /* value of -n mod p^e */

            if (a_mod == 0) {                      /* 0 is always a square */
                continue;
            }

            if (p == 2) {
                /* test residue modulo 2^e */
                if (!is_qr_mod_2pow(a_mod, e)) {
                    square_residue = false;
                    break;
                }
            } else {
                /* odd prime power */
                /* valuation of a_mod at p */
                idx_t v = 0;
                while (a_mod % p == 0) {
                    a_mod /= p;
                    ++v;
                }
                if (v % 2 == 1) {                  /* odd valuation ⇒ not a square */
                    square_residue = false;
                    break;
                }
                /* now a_mod is odd and coprime to p */
                if (jacobi_symbol((uint64_t)a_mod, p) != 1) {
                    square_residue = false;
                    break;
                }
            }
        }

        if (square_residue) ++count;
    }

    return count;
}
