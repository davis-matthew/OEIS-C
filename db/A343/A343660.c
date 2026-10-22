/*
 * A343660  Number of maximal pairwise coprime sets of at least two divisors >
 *          1 of n.
 * https://oeis.org/A343660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π e_i × (Bell(k) – 1) where n = ∏ p_i^e_i, k = number of
 *            distinct prime factors, and Bell(k) is the k‑th Bell number
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* maximal pairwise coprime sets of at least two divisors > 1 of n */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,
                                    uint64_t *pr, uint64_t *ex, size_t *cnt); */

static const seq_t bell_numbers[6] = {1, 1, 2, 5, 15, 52};

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* no divisors > 1 */

    /* factor n: largest n up to 10000 => at most 6 distinct primes */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    if (cnt == 0) return 0;        /* only possible for n = 1 */

    /* product of exponents */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (seq_t)exps[i];
    }

    /* Bell number for k = cnt */
    seq_t result = 0;
    if (cnt <= 5) {                 /* means n ≤ 10000, cnt ≤ 5 */
        result = prod * ((seq_t)bell_numbers[cnt] - 1);
    } else {                        /* safe fallback: compute Bell(k) recursively */
        /* compute Bell number using dynamic programming */
        seq_t *B = (seq_t*)calloc(cnt + 1, sizeof(seq_t));
        if (B) {
            B[0] = 1;
            for (size_t k = 1; k <= cnt; ++k) {
                seq_t sum = 0;
                for (size_t j = 0; j < k; ++j) {
                    /* Stirling numbers of the second kind S(k,j) */
                    seq_t stirling = 1;
                    for (size_t l = 0; l < j; ++l)
                        stirling = stirling * (k - l) / (l + 1);
                    sum += stirling * B[j];
                }
                B[k] = sum;
            }
            result = prod * (B[cnt] - 1);
            free(B);
        }
    }
    return result;
}
