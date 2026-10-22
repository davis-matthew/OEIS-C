/*
 * A330404  Least nonsquare k that is a quadratic residue modulo n.
 * https://oeis.org/A330404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 7022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least nonsquare quadratic residue modulo n.
   a(n) = min{ k > 0 | k is a quadratic residue (mod n) and k is not a perfect square }  */
/* Includes -------------------------------------------------------------- */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* Helper --------------------------------------------------------------- */
static inline bool is_square_seq(seq_t x) {
    seq_t s = floor_sqrt(x);
    return s * s == x;
}

/* Main sequence function ----------------------------------------------- */
seq_t a(idx_t n) {
    /* n >= 1 in the tests.  Convert to local variable to avoid accidental
       unsigned wrap‑around when doing n-1 or loops. */
    idx_t N = n;

    /* Mark quadratic residues modulo N.  0 <= r < N. */
    bool *qr = (bool *)calloc((size_t)N, sizeof(bool));
    if (!qr) return (seq_t)0;      /* safety – should not happen in the tests */

    for (idx_t x = 0; x < N; ++x) {
        seq_t r = (seq_t)x * (seq_t)x % (seq_t)N;
        qr[(size_t)r] = true;
    }

    seq_t best = 0;
    bool found = false;

    for (idx_t r = 0; r < N; ++r) {
        if (!qr[(size_t)r]) continue;     /* not a quadratic residue */

        seq_t cand;

        if (r == 0) {
            /* Multiple of N.  The smallest non‑square multiple is N
               (if N is not a square) or 2N otherwise. */
            if (!is_square_seq((seq_t)N))
                cand = (seq_t)N;
            else
                cand = (seq_t)2 * (seq_t)N;
        } else {
            /* Residue r itself or a single step upwards. */
            cand = (seq_t)r;
            if (is_square_seq(cand)) {
                /* Search the arithmetic progression r + t*N for the first
                   non‑square.  The number of iterations is very small in practice
                   because squares grow quickly. */
                idx_t t = 1;
                while (true) {
                    cand = (seq_t)r + (seq_t)t * (seq_t)N;
                    if (!is_square_seq(cand))
                        break;
                    ++t;
                }
            }
        }

        if (!found || cand < best) {
            best = cand;
            found = true;
        }
    }

    free(qr);
    return best;
}
