/*
 * A383228  a(n) is the number of cases where both j and k (1 <= j < k <= n),
 *          are divisors of Sum_{i=j..k} i^i.
 * https://oeis.org/A383228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A383228
   a(n) = number of pairs (j,k) with 1 ≤ j < k ≤ n such that
   j | Σ_{i=j..k} i^i  and  k | Σ_{i=j..k} i^i
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* fast modular exponentiation: base^exp mod mod
   (mod ≤ 10000, base ≤ 10000) */
static inline uint32_t modpow_uint32(uint32_t base, idx_t exp, uint32_t mod)
{
    if (mod == 1U) return 0U;          /* anything mod 1 is 0 */
    uint32_t result = 1U % mod;
    uint32_t b = base % mod;
    for (idx_t e = exp; e; e >>= 1U) {
        if (e & 1U)
            result = (uint32_t)((uint64_t)result * b % mod);
        b = (uint32_t)((uint64_t)b * b % mod);
    }
    return result;
}

/* a(n) as described above */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;       /* no j<k pairs */

    /* array of current sums modulo each j (1 <= j <= n) */
    uint32_t *sum_mod = (uint32_t*)calloc((size_t)(n + 1), sizeof(uint32_t));
    if (!sum_mod) return (seq_t)0;     /* allocation failed */

    uint64_t count = 0;                /* result fits in 64 bits */
    for (idx_t k = 1; k <= n; ++k) {
        /* update each sum_mod[j] by adding k^k mod j */
        for (idx_t j = 1; j <= k; ++j) {
            uint32_t add = modpow_uint32((uint32_t)k, k, (uint32_t)j);
            uint32_t tmp = sum_mod[j] + add;
            if (tmp >= j) tmp -= j;    /* sum_mod[j] < j, add < j   */
            sum_mod[j] = tmp;
        }

        if (k < 2) continue;          /* no j < k yet */
        for (idx_t j = 1; j < k; ++j) {
            if (sum_mod[j] == 0) {     /* j divides the sum_{i=j..k} i^i */
                /* compute the same sum modulo k */
                uint64_t acc = 0;
                for (idx_t i = j; i <= k; ++i)
                    acc += modpow_uint32((uint32_t)i, i, (uint32_t)k);
                if ((acc % k) == 0)
                    ++count;           /* k also divides the sum */
            }
        }
    }

    free(sum_mod);
    return (seq_t)count;
}
