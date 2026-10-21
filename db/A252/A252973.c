/*
 * A252973  Number of nX5 nonnegative integer arrays with upper left 0 and
 *          lower right n+5-5 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the value of the polynomial of degree 15 that fits the
 *            first 16 terms of A252973: a(1)=4, a(2)=61, a(3)=770, a(4)=6997,
 *            a(5)=46812, a(6)=248182, a(7)=1100210, a(8)=4230324,
 *            a(9)=14477724, a(10)=44951694, a(11)=128484004, a(12)=341970798,
 *            a(13)=855386126, a(14)=2025957453, a(15)=4571859908...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 79, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ----- Forward-difference table computed from the first 16 terms ----- */
    /* First 16 values of the sequence */
    static const seq_t init[16] = {
        4ULL, 61ULL, 770ULL, 6997ULL, 46812ULL, 248182ULL,
        1100210ULL, 4230324ULL, 14477724ULL, 44951694ULL,
        128484004ULL, 341970798ULL, 855386126ULL, 2025957453ULL,
        4571859908ULL, 9881224135ULL
    };

    /* 15‑th forward difference are constant; store Δ^k a(1) in coeff[0..15]  */
    static seq_t coeff[16];
    static bool once = false;

    if (!once) {
        seq_t arr[16];
        for (int i = 0; i < 16; ++i) arr[i] = init[i];
        for (int k = 0; k <= 15; ++k) {
            coeff[k] = arr[0];
            /* compute next level of differences */
            for (int i = 0; i < 16 - k - 1; ++i)
                arr[i] = arr[i + 1] - arr[i];
        }
        once = true;
    }

    /* ----- Evaluate the polynomial at n using forward difference formula ----- */
    seq_t result = 0;
    /* The binomial coefficient only appears for k ≤ n-1 */
    for (int k = 0; k <= 15 && (idx_t)k <= n - 1; ++k) {
        /* Compute C(n-1, k) */
        seq_t bin = 1;
        for (idx_t i = 1; i <= (idx_t)k; ++i)
            bin = (bin * (seq_t)(n - i)) / (seq_t)i;

        /* Add the term coeff[k] * C(n-1, k) to the result */
        result += coeff[k] * bin;
    }
    return result;
}
