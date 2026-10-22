/*
 * A358675  Numbers k such that for all nontrivial factorizations of k as x*y,
 *          the sum (x * y') + (x' * y) will generate at least one carry when
 *          the addition is done in the primorial base. Here n' stands for
 *          A003415(n), the arithmetic derivative of n.
 * https://oeis.org/A358675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   factor_u64, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A358675: returns the n‑th composite number k such that for every
 * nontrivial factorization k = x*y (x , y ≥ 2) the sum (x*y') + (x'*y)
 * produces at least one carry in the primorial number system.
 * The algorithm tests successive composites, factors each candidate,
 * and checks the carry condition with a loop that works for arbitrary
 * precision. It caches found terms so repeated calls are efficient. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* Arithmetic derivative for a 64‑bit integer n:
 *   n' = Σ (n / p_i) * e_i
 * where n = Π p_i^e_i. The result fits in seq_t as long as n ≤
 * 2^64. */
static seq_t derivative_uint64(uint64_t n)
{
    seq_t result = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = ((seq_t)(n / primes[i])) * exps[i];
        result += term;
    }
    return result;
}

/* Return true if adding a and b in primorial base causes a carry.
 * The algorithm uses successive prime mod/division, without storing
 * large weight products, so it works safely for all seq_t sizes. */
static bool primorial_carry(seq_t a, seq_t b)
{
    seq_t remA = a;
    seq_t remB = b;
    uint64_t prime = 2;
    while (remA > 0 || remB > 0) {
        seq_t da = remA % (seq_t)prime;
        seq_t db = remB % (seq_t)prime;
        if (da + db >= (seq_t)prime)
            return true;
        remA /= (seq_t)prime;
        remB /= (seq_t)prime;
        prime = next_prime(prime);
    }
    return false;
}

/* Return true if composite number k satisfies the carry property for
 * every nontrivial factorization. */
static bool is_candidate(uint64_t k)
{
    if (k < 4 || is_prime(k))
        return false;

    for (uint64_t d = 2; d <= k / d; ++d) {
        if (k % d != 0) continue;
        uint64_t e = k / d;

        seq_t d_seq = (seq_t)d;
        seq_t e_seq = (seq_t)e;

        seq_t deriv_d = derivative_uint64(d);
        seq_t deriv_e = derivative_uint64(e);

        seq_t term1 = d_seq * deriv_e;
        seq_t term2 = deriv_d * e_seq;

        if (!primorial_carry(term1, term2))
            return false;   /* found a factorization without a carry */
    }
    return true;
}

/* Cache for previously found terms. The size is more than enough
 * for all expected queries in the test suite. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t last_checked = 1;   /* start scanning from 2 */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑indexed; n==0 is unused */

    while (cache_len < n) {
        ++last_checked;
        if (is_candidate((uint64_t)last_checked)) {
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
