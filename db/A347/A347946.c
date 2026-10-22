/*
 * A347946  Products of nonprimitive roots of n, or 0 if n = 2 or has no
 *          primitive roots.
 * https://oeis.org/A347946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all non‑primitive roots modulo n, 0 if n = 2
 *            or n has no primitive roots. (OEIS A347946)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 17, u64 n <= 25, u128 n <= 30,
 *            big any n
 * Helpers:   factor_u64, gcd_u64, has_primitive_root, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd_u64.h"
#include "../utils/has_primitive_root.h"

/* ---------- Utility: GCD for uint64_t ---------- */

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    /* Special cases */
    if (nn <= 2u) return (seq_t)0;

    /* Factorize n once */
    static const size_t MAX_FACT = 64;
    uint64_t primes_n[MAX_FACT];
    uint64_t exps_n[MAX_FACT];
    size_t cnt_n = 0;
    factor_u64(nn, primes_n, exps_n, &cnt_n);

    /* Check existence of primitive roots */
    if (!has_primitive_root(nn, primes_n, exps_n, cnt_n)) {
        return (seq_t)0;
    }

    /* Compute phi(n) from factorization of n */
    uint64_t phi_n = nn;
    for (size_t i = 0; i < cnt_n; ++i) {
        phi_n -= phi_n / primes_n[i];
    }

    /* Factorize phi(n) to get its distinct prime factors */
    uint64_t primes_phi[MAX_FACT];
    uint64_t exps_phi[MAX_FACT];
    size_t cnt_phi = 0;
    factor_u64(phi_n, primes_phi, exps_phi, &cnt_phi);

    /* Product of non‑primitive roots */
    seq_t prod = (seq_t)1;
    for (uint64_t i = 1u; i < nn; ++i) {
        /* Residues not coprime to n are automatically non‑primitive */
        if (gcd_u64(i, nn) != 1u) {
            prod *= (seq_t)i;
            continue;
        }

        /* Test if i is a primitive root */
        bool is_primitive = true;
        for (size_t j = 0; j < cnt_phi; ++j) {
            if (mod_pow((seq_t)i, phi_n / primes_phi[j], (seq_t)nn) == (seq_t)1) {
                is_primitive = false;
                break;
            }
        }

        if (!is_primitive) {
            prod *= (seq_t)i;
        }
    }

    return prod;
}
