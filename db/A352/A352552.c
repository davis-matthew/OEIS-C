/*
 * A352552  a(n) = number of modules with n elements over the ring of integers
 *          in the real quadratic field of discriminant 12.
 * https://oeis.org/A352552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352552
 * a(n) = number of R-modules of size n, where R = ℤ[√3] (discriminant 12).
 * For n ≥ 1, factor n = ∏ pᵉ:
 *   • If p = 2 or 3       (ramified)  → contribution = P(e),   the number of partitions of e.
 *   • If p ≡ 1 or 11 (mod 12) (split)   → contribution = Σ_{k=0..e} P(k)·P(e−k).
 *   • If p ≡ 5 or 7  (mod 12) (inert)   → contribution = 0 if e odd, otherwise P(e/2).
 * The sequence value is the product of these contributions.
 */

#include <stdbool.h>

static seq_t part_tab[14];
static bool part_inited = false;

/* Precompute partition numbers P(0)…P(13) */
static void init_partitions(void) {
    part_tab[0] = 1;
    for (int i = 1; i <= 13; i++) {
        for (int j = i; j <= 13; j++) {
            part_tab[j] += part_tab[j - i];
        }
    }
    part_inited = true;
}

/* local factor for a prime p with exponent e */
static seq_t local_factor(idx_t p, idx_t e) {
    if (!part_inited) init_partitions();

    if (p == 2 || p == 3)        /* ramified primes */
        return part_tab[e];

    int mod12 = p % 12;
    if (mod12 == 1 || mod12 == 11) {          /* split */
        seq_t sum = 0;
        for (idx_t k = 0; k <= e; k++)
            sum += part_tab[k] * part_tab[e - k];
        return sum;
    } else {                                 /* inert */
        if (e & 1)            /* odd exponent impossible */
            return 0;
        return part_tab[e / 2];
    }
}

seq_t a(idx_t n) {
    if (n == 0)                 /* offset not used, but guard */
        return 0;

    if (!part_inited) init_partitions();

    seq_t result = 1;
    idx_t tmp = n;
    idx_t e;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            e++;
        }
        result *= local_factor(2, e);
    }

    /* factor 3 */
    if (tmp % 3 == 0) {
        e = 0;
        while (tmp % 3 == 0) {
            tmp /= 3;
            e++;
        }
        result *= local_factor(3, e);
    }

    /* remaining odd primes */
    for (idx_t p = 5; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            result *= local_factor(p, e);
        }
    }

    /* last prime factor > 1 */
    if (tmp > 1)
        result *= local_factor(tmp, 1);

    return result;
}
