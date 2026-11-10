/*
 * A385987  Prime numbers of the form A385986(1) + ... + A385986(k) for some k
 *          > 0.
 * https://oeis.org/A385987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime among partial sums T(k) =
 *            A385986(1)+...+A385986(k); A385986(1)=2, A385986(m) = largest
 *            j<m with T(j) prime. Computed by direct simulation
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 5644, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 8717 (2026-11-10)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t last = 0;          /* largest index j with T(j) prime so far */
    bool prev_prime = false; /* is T(k-1) prime? */
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t cur;
        if (k == 1) {
            cur = 2;
        } else {
            if (prev_prime) last = k - 1;
            cur = (seq_t)last;
        }
        sum += cur;
        prev_prime = is_prime_seq(sum);
        if (prev_prime) {
            ++count;
            if (count == n) return sum;
        }
    }
}
