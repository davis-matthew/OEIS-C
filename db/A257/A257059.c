/*
 * A257059  Number of length n 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257059.  For an array of length n with entries 1..n+1,
   count the arrays such that every prefix sum is divisible by 2 or 3.
   The computation uses dynamic programming on residues modulo 6.
   Allowed residues for a prefix sum (mod 6) are 0,2,3,4.  The
   transition matrix M is built from the counts of numbers 1..n+1
   having each residue; the answer is the sum of the first row of Mⁿ. */

#include <stdint.h>

/* 4×4 matrix over seq_t */
struct Mat {
    seq_t a[4][4];
};

/* Matrix multiplication: C = A × B */
static void mat_mul(const struct Mat *A, const struct Mat *B, struct Mat *C)
{
    for (idx_t i = 0; i < 4; ++i) {
        for (idx_t j = 0; j < 4; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 4; ++k)
                sum += A->a[i][k] * B->a[k][j];
            C->a[i][j] = sum;
        }
    }
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    /* Allowed residues modulo 6 for a prefix sum */
    static const idx_t ALLOWED[4] = {0, 2, 3, 4};

    /* Count how many numbers 1..n+1 have each residue modulo 6 */
    seq_t counts[6];
    seq_t alphabet = (seq_t)(n + 1);
    seq_t base = alphabet / 6;
    seq_t rem = alphabet % 6;            /* 0..5 */
    for (idx_t r = 0; r < 6; ++r) {
        counts[r] = base;
        if (r != 0 && r <= rem)         /* residues 1..rem appear once more */
            counts[r] += 1;
    }

    /* Build transition matrix M over the allowed residues */
    struct Mat M = {0};
    for (idx_t i = 0; i < 4; ++i) {
        idx_t from = ALLOWED[i];
        for (idx_t j = 0; j < 4; ++j) {
            idx_t to   = ALLOWED[j];
            idx_t diff = (to + 6 - from) % 6;
            M.a[i][j] = counts[diff];
        }
    }

    /* Identity matrix */
    struct Mat I = {0};
    for (idx_t i = 0; i < 4; ++i)
        I.a[i][i] = 1;

    /* Fast exponentiation: compute Mⁿ */
    struct Mat res = I;
    struct Mat pow = M;
    struct Mat tmp;
    idx_t e = n;
    while (e > 0) {
        if (e & 1ULL) {
            mat_mul(&res, &pow, &tmp);
            res = tmp;
        }
        mat_mul(&pow, &pow, &tmp);
        pow = tmp;
        e >>= 1;
    }

    /* Sum of the first row of Mⁿ gives the number of valid arrays */
    seq_t ans = 0;
    for (idx_t j = 0; j < 4; ++j)
        ans += res.a[0][j];
    return ans;
}
