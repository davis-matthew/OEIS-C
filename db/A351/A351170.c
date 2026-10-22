/*
 * A351170  Consider the primes of the form p(m)=m^2+1 such that p(m+2) is
 *          also prime for some m. The sequence lists the sums p(m) + p(m+2).
 * https://oeis.org/A351170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*(m_n+1)^2+4 where m_n is the n‑th integer m such that
 *               m^2+1 and (m+2)^2+1 are prime.
 * Computation: iterate m, test primes with isPrimeGeneric, memoise results.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;
    static size_t  vals_len = 0;
    static size_t  vals_cap = 0;
    static uint64_t next_m  = 1;     /* next m to test */

    if (n == 0) return 0;           /* offset starts at 1 */

    size_t target = (size_t)n;
    if (vals_len < target) {
        if (vals_cap < target) {
            size_t new_cap = vals_cap ? vals_cap * 2 : 1;
            if (new_cap < target) new_cap = target;
            ensure_capacity(new_cap, &vals_cap, (void **)&vals, sizeof(seq_t));
        }
        while (vals_len < target) {
            seq_t m  = (seq_t)next_m;
            seq_t p1 = m * m + 1;
            seq_t p2 = (m + 2) * (m + 2) + 1;

            if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
                seq_t mm1 = m + 1;
                seq_t val = 2 * (mm1 * mm1) + 4;
                vals[vals_len++] = val;
            }
            ++next_m;
        }
    }
    return vals[n - 1];
}
