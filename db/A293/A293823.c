/*
 * A293823  Number of integer-sided hexagons having perimeter n, modulo
 *          rotations but not reflections.
 * https://oeis.org/A293823
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   Counts integer‑sided hexagons of perimeter n * up to rotations
 *            (reflections distinct), using Burnside's lemma. * a(n) =
 *            (C(n-1,5) - 6*C(n-ceil(n/2),5) * + 2*δ(n≡0 mod 6) * + 2*(n/3 - 1
 *            if n≡0 mod 3 else 0) * + (C(n/2-1,2) if n even else 0)) / 6 *
 *            where Δ is the indicator function
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* small binomial coefficient C(N,K) for K ≤ 5; uses seq_t for result */
static inline seq_t binom_small(idx_t N, unsigned K)
{
    if (N < K) return 0;
    seq_t res = 1;
    for (unsigned i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i);
        res = res / i;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 6) return 0;           /* below offset, not used */
    seq_t N = (seq_t)n;

    /* total positive integer 6‑tuples summing to n */
    seq_t total = binom_small(n - 1, 5);

    /* subtract tuples with a side ≥ ceil(n/2) */
    idx_t t = (n % 2 == 0) ? (n / 2) : ((n + 1) / 2); /* ceil(n/2) */
    idx_t mult = n - t;
    seq_t bad = 0;
    if (mult >= 5) {
        bad = 6 * binom_small(mult, 5);
    }

    seq_t id = total - bad;

    /* fixed points of other rotations */
    seq_t rot1 = (n % 6 == 0) ? 1 : 0;           /* rotations by 1 or 5 */
    seq_t rot3 = 0;
    if ((n % 2) == 0) {                          /* rotation by 3 */
        rot3 = binom_small((n / 2) - 1, 2);
    }

    seq_t rot23 = 0;                             /* rotations by 2 or 4 */
    if ((n % 3) == 0) {
        rot23 = (seq_t)(n / 3 - 1);               /* number of (x,y) with x+y=n/3 */
    }

    /* Burnside sum */
    seq_t sum = id
                + 2 * rot1
                + 2 * rot23
                + rot3;

    return sum / 6;
}
