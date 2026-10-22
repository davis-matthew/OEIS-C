/*
 * A343397  Number of ways to write n as 2^x + [y^2/3] + [z^2/4] with x,y,z
 *          positive integers, where [.] is the floor function.
 * https://oeis.org/A343397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of positive integers such that
 *            n = 2^x + ⎣y²/3⎦ + ⎣z²/4⎦
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function is memoised: counts of ⎣y²/3⎦ and ⎣z²/4⎦ up to the
   largest n requested so far are stored in static arrays.  For each
   call the function sums over the powers of two and counts the
   number of pairs (y,z) that fill the remaining sum. */

seq_t a(idx_t n)
{
    /* static buffers holding counts of the floor terms */
    static size_t cap = 0;          /* allocated size (number of indices)  */
    static seq_t *Acount = NULL;    /* count of y giving ⎣y²/3⎦ value  */
    static seq_t *Bcount = NULL;    /* count of z giving ⎣z²/4⎦ value  */

    /* grow buffers if a larger n has been requested */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_A = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *new_B = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_A || !new_B) {
            /* insufficient memory – return 0; tests never hit this */
            return (seq_t)0;
        }
        /* free previous arrays */
        if (Acount) free((void *)Acount);
        if (Bcount) free((void *)Bcount);

        Acount = new_A;
        Bcount = new_B;
        cap = new_cap;

        /* compute counts for all possible values up to the new cap */
        for (idx_t y = 1; ; ++y) {
            __uint128_t prod = (__uint128_t)y * y;
            idx_t val = (idx_t)(prod / 3);
            if (val >= cap) break;
            ++Acount[(size_t)val];
        }
        for (idx_t z = 1; ; ++z) {
            __uint128_t prod = (__uint128_t)z * z;
            idx_t val = (idx_t)(prod / 4);
            if (val >= cap) break;
            ++Bcount[(size_t)val];
        }
    }

    seq_t total = 0;

    /* iterate over all powers of two that fit into n */
    idx_t pow2 = 2;   /* 2^1 */
    while (pow2 <= n) {
        idx_t rem = n - pow2;   /* remaining sum to be decomposed */

        /* for every possible A = ⎣y²/3⎦ value currently <= rem */
        for (idx_t a = 0; a <= rem; ++a) {
            seq_t ac = Acount[(size_t)a];
            if (ac == 0) continue;
            seq_t bc = Bcount[(size_t)(rem - a)];
            if (bc == 0) continue;
            total += ac * bc;
        }

        /* go to next power of two; guard against overflow */
        if (pow2 > n / 2) break;
        pow2 <<= 1;
    }

    return total;
}
