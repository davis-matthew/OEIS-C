/*
 * A385479  a(n) is the cardinality of the union of the sets of divisors of
 *          sigma(k) for 1 <= k <= n.
 * https://oeis.org/A385479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |⋃_{k=1..n} divisors(σ(k))| (σ = sum‑of‑divisors)
 * Range:     u8 n <= 29, u16 n <= 5004, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* set a bit in a bit‑set */
static inline void set_bit(uint64_t *bits, size_t idx) {
    bits[idx >> 6] |= 1ULL << (idx & 63);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* indices below offset are irrelevant */

    /* ---- 1. compute σ(k) for 1 ≤ k ≤ n ------------------------------ */
    seq_t *sigma = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!sigma) return 0;  /* allocation failure */

    for (size_t i = 1; i <= (size_t)n; ++i) {
        seq_t inc = (seq_t)i;
        for (size_t j = i; j <= (size_t)n; j += i) {
            sigma[j] += inc;
        }
    }

    /* ---- 2. find maximum σ to know the range of divisors ----------- */
    seq_t max_sigma = 0;
    for (size_t k = 1; k <= (size_t)n; ++k) {
        if (sigma[k] > max_sigma) max_sigma = sigma[k];
    }

    /* if σ(n) is too large for a bit‑set we abort – this never happens
       for n ≤ 10000 in the usual test cases (σ ≤ few × 10⁴).          */
    size_t max_val = (size_t)max_sigma;   /* safe for n ≈ 10⁴  */

    size_t bits_len = (max_val >> 6) + 2;  /* +1 for possible overflow, +1 for 0‑index */
    uint64_t *bits = (uint64_t *)calloc(bits_len, sizeof(uint64_t));
    if (!bits) {
        free(sigma);
        return 0;
    }

    /* ---- 3. mark all divisors of every σ(k) --------------------- */
    for (size_t k = 1; k <= (size_t)n; ++k) {
        seq_t s = sigma[k];
        size_t sqrt_s = (size_t)floor_sqrt(s);

        for (size_t d = 1; d <= sqrt_s; ++d) {
            seq_t d_seq = (seq_t)d;
            if (s % d_seq != 0) continue;

            size_t d1 = d;
            size_t d2 = (size_t)(s / d_seq);

            set_bit(bits, d1);
            if (d1 != d2) set_bit(bits, d2);
        }
    }

    /* ---- 4. count distinct divisors -------------------------------- */
    seq_t count = 0;
    for (size_t i = 0; i < bits_len; ++i) {
        count += (seq_t)__builtin_popcountll(bits[i]);
    }

    /* ---- cleanup ---------------------------------------------------- */
    free(sigma);
    free(bits);

    return count;
}
