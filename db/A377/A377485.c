/*
 * A377485  Irregular triangle where row n lists powers p^k of primes p | n
 *          such that p^k <= n and k > 0.
 * https://oeis.org/A377485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the (n‑th) term of the flattened irregular triangle *
 *            whose row i (i ≥ 1) consists of all prime powers p^k such that *
 *            p divides i, k ≥ 1, and p^k ≤ i, listed in increasing order
 * Range:     u8 n <= 623, u16 n <= 8842, u32 n <= 8842, u64 n <= 8842,
 *            u128 n <= 8842, big any n
 * Helpers:   _sort_seqt, _unique_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation computes the sequence on demand by
 * iterating over rows and counting the cumulative number of elements
 * until the given index n is reached.  It uses only integer arithmetic
 * and is fully compatible with any build in which seq_t and idx_t are
 * defined (e.g., uint8_t, uint16_t, uint32_t, uint64_t, or an
 * arbitrary‑precision type). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_sort_seqt.h"
#include "../utils/_unique_prime_factors.h"

/* ------------------------------------------------------------------ */
/* Helper: factor the integer n into its unique prime divisors.      */
/* The function fills the array 'primes' with the primes and returns  */
/* the number of distinct primes found.  The array must have space   */
/* for at least 16 primes, which is sufficient for any n encountered  */
/* in typical test ranges (e.g., n ≤ 10⁶). Most rows in practice are */
/* < 10000, where fewer primes occur.                               */

/* ------------------------------------------------------------------ */
/* Insertion sort for a small array of seq_t values.  The array size  */
/* is tiny (< 64), so this simple algorithm is adequate.             */

/* ------------------------------------------------------------------ */
/* Main sequence function                                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Sequence is 1‑indexed: a(1) is the first element. */
    if (n == 0) /* undefined case – guard against underflow */
        return (seq_t)0;

    /* Special case for the very first term */
    if (n == 1)
        return (seq_t)1;

    /* The target index we are looking for after flattening rows. */
    idx_t target = n;

    /* Storage for prime powers of the current row; 64 is plenty. */
    seq_t powers[64];

    /* Storage for unique prime divisors of the current row. */
    uint64_t primes[16];
    size_t prime_cnt;

    for (idx_t row = 1; ; ++row) {
        size_t len = 0;          /* number of elements in this row */

        if (row == 1) {
            powers[len++] = (seq_t)1;
        } else {
            /* Factor the current row into its distinct prime divisors. */
            prime_cnt = _unique_prime_factors(row, primes, 16);

            for (size_t pi = 0; pi < prime_cnt; ++pi) {
                seq_t base = (seq_t)primes[pi];
                seq_t val = base;

                /* Generate powers base^k <= row, ensuring no overflow. */
                while (1) {
                    powers[len++] = val;
                    if (val > (seq_t)row / base)
                        break;
                    val = val * base;
                }
            }
            /* Sort the powers in ascending order. */
            _sort_seqt(powers, len);
        }

        if (target <= (idx_t)len)
            return powers[target - 1];

        target -= (idx_t)len;
    }
}
