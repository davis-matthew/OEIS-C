/*
 * A245615  Sum_{lpf(j)=11, 11<j<=11*prime(n)}(-1)^(A000120(j)+1), where
 *          lpf=least prime factor.
 * https://oeis.org/A245615
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{lpf(j)=11, 11<j≤11·prime(n)} (-1)^(A000120(j)+1)
 * Range:     u8 n <= 1741, u16 n <= 4905, u32 n <= 4905, u64 n <= 4905,
 *            u128 n <= 4905, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The summation is over j = 11·k with 2 ≤ k ≤ prime(n) where k is not
    divisible by any prime smaller than 11 (i.e., not divisible by 2,3,5 or 7). */
/*  Implementation uses only integer arithmetic, the helper function
    get_prime() from the utils library, and the GCC builtin popcount. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 5; values for n < 5 are irrelevant. */
    if (n < 5U)
        return (seq_t)0;

    /* upper bound for k :  k ≤ prime(n)  */
    idx_t prime_n = (idx_t)get_prime(n);

    long long sum = 0;   /* use signed to accumulate ±1 terms */

    for (idx_t k = 2U; k <= prime_n; ++k) {
        /* Reject k that has a prime factor < 11. */
        if ((k & 1U) == 0U || k % 3ULL == 0ULL || k % 5ULL == 0ULL || k % 7ULL == 0ULL)
            continue;

        /* j = 11 · k; it fits in 64‑bit for all n we consider. */
        uint64_t j = 11ULL * (uint64_t)k;

        /* popcount of j */
        unsigned int pop = __builtin_popcountll(j);

        /* add ±1 according to parity of popcount */
        sum += (pop & 1U) ? 1 : -1;
    }

    return (seq_t)sum;
}
