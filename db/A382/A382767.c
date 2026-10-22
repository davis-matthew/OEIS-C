/*
 * A382767  Smallest number k that is powerful but not a prime power that is
 *          also coprime to n.
 * https://oeis.org/A382767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p² * q², where p and q are the smallest primes coprime to
 *            n. (OEIS A382767)
 * Range:     u8 n <= 5, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Primitive primality test sufficient for the small candidate primes
   we encounter when looking for the two smallest primes coprime to n. */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/*
   Compute a(n) for arbitrary n >= 1.
   The function scans successive integers starting at 2,
   picks the first two that are prime and not dividing n,
   and returns the square of their product.
*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, safe return */

    idx_t first = 0, second = 0;
    idx_t cand = 2;

    while (second == 0) {
        if (is_prime(cand) && n % cand != 0) {
            if (first == 0)
                first = cand;
            else {
                second = cand;
                break;
            }
        }
        ++cand;
    }

    seq_t prod = (seq_t)first * (seq_t)second;
    return prod * prod;   /* (p * q)² */
}
