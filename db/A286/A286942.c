/*
 * A286942  Irregular triangle read by rows: numbers 1 <= k <= (A002110(n) -
 *          1) where gcd(k, A002110(n - 1)) = 1.
 * https://oeis.org/A286942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 6839, u32 n <= 6839, u64 n <= 6839,
 *            u128 n <= 6839, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS sequence A286942: Irregular triangle read by rows.
 *  Numbers 1 <= k <= (A002110(n) - 1) where gcd(k, A002110(n-1)) = 1.
 *  A002110(n) is the product of the first n primes (primorial),
 *  with the convention A002110(0) = 1.
 *  The sequence is written row‑by‑row; this function returns the
 *  n-th term (1‑based) of that flattened triangle.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

/* product of the first n primes;   n==0 -> 1 */
static seq_t primorial(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 0; i < n; ++i)
        prod *= (seq_t)primes[i];
    return prod;
}

/* Return the n-th term (1‑based) of A286942. */
seq_t a(idx_t n)
{
    /* first two terms are fixed by the triangle definition */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    idx_t target = n;          /* required position in the flattened list */
    idx_t seen   = 2;          /* the two terms already accounted for */
    idx_t row    = 2;          /* we start generating rows from row 2 */

    while (true) {
        /* B_{row-1} = primorial(row-1), B_{row} = primorial(row) */
        seq_t B_prev = primorial(row - 1);
        seq_t B_curr = primorial(row);
        idx_t limit  = (idx_t)(B_curr - 1);

        /* primes dividing B_prev are the first (row-1) primes */
        for (idx_t k = 1; k <= limit; ++k) {
            bool coprime = true;
            for (idx_t j = 0; j < row - 1; ++j)
                if (k % primes[j] == 0) { coprime = false; break; }

            if (coprime) {
                ++seen;
                if (seen == target)
                    return (seq_t)k;
            }
        }
        ++row;
    }
}
