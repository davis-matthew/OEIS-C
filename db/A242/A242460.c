/*
 * A242460  Integers in the Hadron Collider: Composite integers, n, break
 *          apart into a(n)>=2 factors in order to minimize the product:
 *          (number of factors)*(max factor - min factor). In case of ties,
 *          a(n) is chosen to be as small as possible.
 * https://oeis.org/A242460
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4436, u16 n <= 9305, u32 n <= 9305, u64 n <= 9305,
 *            u128 n <= 9305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A242460: For n ≥ 2, a(n) is the minimal number of factors k in a factorisation
 * of n into integers > 1 such that k * (max factor – min factor) is minimised.
 * If several factorizations attain the minimal product, a(n) is the smallest such k.
 * Primes and prime‑powers with even exponent have a(n)=2 or the minimal exponent >1
 * for odd exponents (e.g. a(8)=3).  This implementation enumerates all
 * factorizations by depth–first search and keeps the best value.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void dfs(uint64_t n, uint64_t start,
                seq_t cur_min, seq_t cur_max,
                size_t depth,
                seq_t *best_prod, size_t *best_k, bool *best_set)
{
    /* enumerate divisors d in non‑decreasing order */
    for (uint64_t d = start; d * d <= n; ++d) {
        if (n % d != 0) continue;
        seq_t d_seq = (seq_t)d;
        seq_t new_min = (depth == 0) ? d_seq : ((d_seq < cur_min) ? d_seq : cur_min);
        seq_t new_max = (depth == 0) ? d_seq : ((d_seq > cur_max) ? d_seq : cur_max);
        dfs(n / d, d, new_min, new_max, depth + 1,
            best_prod, best_k, best_set);
    }

    /* use n itself as the last factor */
    if (n >= start) {
        seq_t n_seq = (seq_t)n;
        seq_t new_min = (depth == 0) ? n_seq : ((n_seq < cur_min) ? n_seq : cur_min);
        seq_t new_max = (depth == 0) ? n_seq : ((n_seq > cur_max) ? n_seq : cur_max);
        size_t new_depth = depth + 1;
        if (new_depth >= 2) {
            seq_t prod = (seq_t)new_depth * (new_max - new_min);
            if (!*best_set ||
                prod < *best_prod ||
                (prod == *best_prod && new_depth < *best_k)) {
                *best_set = true;
                *best_prod = prod;
                *best_k = new_depth;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    uint64_t n64 = (uint64_t)n;
    seq_t best_prod = 0;
    size_t best_k = 0;
    bool best_set = false;

    dfs(n64, 2, 0, 0, 0, &best_prod, &best_k, &best_set);

    if (!best_set) return 0;          /* prime numbers */
    return (seq_t)best_k;
}
