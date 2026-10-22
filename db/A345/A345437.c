/*
 * A345437  Represent the ring R = {x+y*sqrt(-2): x, y rational integers} by
 *          the cells centered at the points (x,y) of a square grid; number
 *          the cells of the grid along a counterclockwise square spiral, with
 *          the cells at (0,0) and (1,0) numbered 0, 1. Sequence lists the
 *          index numbers of the cells which are 0...
 * https://oeis.org/A345437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A345437: indices of cells in the square spiral that correspond
 *  to the zero element or to a prime element of the ring ℤ[√‑2].
 *
 *  The function a(n) enumerates the spiral cells and returns the index of
 *  the n‑th cell whose value x + y√‑2 is 0 or a prime element.  It uses
 *  the criterion:  
 *      - N = x² + 2y².  
 *      - N = 0  →  zero element, counts as prime.  
 *      - N = 1  →  unit, not prime.  
 *      - If N is an odd rational prime p and p ≡ 1 or 3 (mod 8), then
 *        (x + y√‑2) is a prime element.  
 *      - If N = p² for a rational prime p with p ≡ 5 or 7 (mod 8), then
 *        (x + y√‑2) = ±p is a prime element.  
 *
 *  The implementation relies on the utility functions:
 *      is_prime(idx_t x)           – primality test for idx_t
 *      int_sqrt(idx_t x)           – integer floor square root
 *
 *  Only the algorithmic part of the sequence definition is implemented;
 *  no lookup tables are used.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/int_sqrt.h"

/*  Return true if the element with norm N in ℤ[√‑2] is zero or a prime. */
static bool is_prime_norm(idx_t N)
{
    if (N == 0)               /* the zero element */
        return true;
    if (N == 1)               /* the units ±1 */
        return false;

    if (is_prime(N)) {        /* N itself is a rational prime */
        if (N == 2 || N % 8 == 1 || N % 8 == 3)
            return true;
        return false;
    }

    /* N might be a square of an inert prime p ≡ 5 or 7 (mod 8) */
    idx_t r = int_sqrt(N);
    if (r * r == N && is_prime(r) && (r % 8 == 5 || r % 8 == 7))
        return true;

    return false;
}

/*  a(n) – index of the n‑th cell that is 0 or a prime in ℤ[√‑2]   */
seq_t a(idx_t n)
{
    /* Spiral traversal state */
    int64_t  x = 0, y = 0;          /* current coordinates   */
    seq_t    index = 0;             /* current index         */
    idx_t    count = 0;             /* number of qualifying cells found   */
    int      dir   = 0;             /* 0=right,1=up,2=left,3=down */
    idx_t    step_len   = 1;        /* steps in current leg    */
    idx_t    steps_left = 1;        /* remaining steps in leg  */
    int      turns = 0;             /* two turns increase leg length */

    while (true) {
        /* Compute norm N = x² + 2y²   */
        idx_t N = (idx_t)((int64_t)x * (int64_t)x)
                 + (idx_t)(2 * (int64_t)y * (int64_t)y);

        if (is_prime_norm(N)) {
            count++;
            if (count == n)
                return index;
        }

        /* Advance to next cell along the spiral */
        switch (dir) {
            case 0: ++x; break;   /* move right  */
            case 1: ++y; break;   /* move up     */
            case 2: --x; break;   /* move left   */
            default: --y; break;  /* move down   */
        }

        index++;
        if (--steps_left == 0) {
            dir = (dir + 1) & 3;  /* turn left */
            turns++;
            if (turns == 2) {
                turns = 0;
                ++step_len;
            }
            steps_left = step_len;
        }
    }
}
