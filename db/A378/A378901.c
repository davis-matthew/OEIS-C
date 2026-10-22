/*
 * A378901  a(n) = 0 if s(n) is nonprime, a(n) = 1 if s(n) is prime, where s =
 *          A005117.
 * https://oeis.org/A378901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the nth squarefree number s(n) is prime, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"     /* provides bool is_prime(idx_t x) */
#include "../utils/is_squarefree_idx.h"

/* OEIS sequence A378901: characteristic function of prime square‑free numbers. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, below not defined */

    idx_t count    = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (is_squarefree_idx(candidate)) {
            ++count;
            if (count == n) {
                /* n-th squarefree number found */
                return is_prime(candidate) ? 1 : 0;
            }
        }
        ++candidate;
    }
    return 0; /* unreachable for n >= 1 */
}
