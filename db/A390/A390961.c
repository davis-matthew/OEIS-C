/*
 * A390961  The sum of the noninfinitary divisors of the numbers whose number
 *          of divisors is not a power of 2 (A162643).
 * https://oeis.org/A390961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the noninfinitary divisors of the n‑th integer
 *            whose number of divisors is not a power of 2
 * Range:     u8 n <= 13, u16 n <= 298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, is_power_of_two, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/is_power_of_two.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Cache of the integers whose divisor count τ(m) is NOT a power of 2 */
    static uint64_t *nums = NULL;
    static size_t nums_cap = 0;
    static size_t nums_len = 0;
    static idx_t next_start = 2;                    /* next integer to test */

    /* Fill cache up to the n‑th such integer */
    while (nums_len < (size_t)n) {
        uint64_t cur = (uint64_t)next_start;
        uint64_t d = divisor_count_u64(cur);
        if (!is_power_of_two((seq_t)d)) {
            if (nums_len == nums_cap) {
                size_t new_cap = nums_cap ? nums_cap << 1 : 64;
                uint64_t *tmp = (uint64_t *)realloc(nums, new_cap * sizeof(uint64_t));
                assert(tmp != NULL);
                nums = tmp;
                nums_cap = new_cap;
            }
            nums[nums_len++] = cur;
        }
        next_start = cur + 1;
    }

    uint64_t m = nums[n - 1];                     /* the required integer */

    /* Factorisation of m */
    uint64_t prime[64], exp[64];
    size_t cnt = 0;
    factor_u64(m, prime, exp, &cnt);
    assert(cnt <= 64);

    /* Compute σ(m) (sum of all divisors) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        uint32_t e = (uint32_t)exp[i];
        seq_t num = pow_uint(p, (idx_t)(e + 1)) - 1;    /* p^(e+1) - 1 */
        seq_t denom = (seq_t)(p - 1);
        seq_t factor = num / denom;                    /* (p^(e+1)-1)/(p-1) */
        sigma *= factor;
    }

    /* Compute sum of infinitary divisors Σ_inf(m) */
    seq_t sum_inf = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = prime[i];
        uint32_t e = (uint32_t)exp[i];
        seq_t factor = 1;
        uint32_t bit = 1;
        while (bit <= e) {
            if (e & bit) {
                seq_t p_pow = pow_uint(p, (idx_t)bit);
                factor *= (1 + p_pow);
            }
            bit <<= 1;
        }
        sum_inf *= factor;
    }

    /* Final result: σ(m) - Σ_inf(m) */
    assert(sigma >= sum_inf);
    return sigma - sum_inf;
}
