/*
 * A338864  Triangle T(n,k) defined by Sum_{k=1..n} T(n,k)*u^k*x^n/n! =
 *          Product_{j>0} ( exp(x^j/(1 - x^j)) )^u.
 * https://oeis.org/A338864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k) defined by T(n,k) =
 *            (n!/k!) * Sum_{i_1+…+i_k = n} ∏_{j=1..k} d(i_j), where d(m) is
 *            the number of divisors of m. Initial values: a(1)=1, a(2)=4,
 *            a(3)=1, a(4)=12, a(5)=12, a(6)=1, …
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_ROW 150   /* 150 rows give 1 + 2 + … + 150 = 11325 ≥ 10000 */

/* Triangle entries (0‑based: T[0][0]=1).  Non‑used slots stay 0. */
static seq_t T[MAX_ROW][MAX_ROW];
/* divisor counts for 1..MAX_ROW-1 */
static uint32_t dcnt[MAX_ROW];
static bool initialized = false;

/* Pre‑compute divisor counts and the triangle T up to MAX_ROW. */
static void init_triangle(void)
{
    if (initialized) return;

    /* Compute divisor counts */
    for (idx_t i = 1; i < MAX_ROW; ++i) dcnt[i] = 0;
    for (idx_t i = 1; i < MAX_ROW; ++i)
        for (idx_t j = i; j < MAX_ROW; j += i) dcnt[j]++;

    /* Triangle */
    T[0][0] = (seq_t)1;
    for (idx_t n = 1; n < MAX_ROW; ++n) {
        T[n][0] = (seq_t)0;           /* k = 0 case is 0 for n>0 */
        T[n][n] = (seq_t)1;           /* k = n gives 1 */

        for (idx_t k = 1; k < n; ++k) {
            seq_t sum = (seq_t)0;
            seq_t P   = (seq_t)1;     /* product_{r=n-m+1}^{n-1} r */
            for (idx_t m = 1; m <= n; ++m) {
                seq_t prev = T[n - m][k - 1];
                if (prev) {
                    seq_t factor = (seq_t)m * (seq_t)dcnt[m];
                    sum += prev * factor * P;
                }
                if (m < n) P = P * (seq_t)(n - m);  /* extend product */
            }
            T[n][k] = sum;
        }
    }

    initialized = true;
}

/* Flattened index -> triangle entry */
seq_t a(idx_t n)
{
    init_triangle();  /* ensure data ready */

    /* Find the row containing the nth term (1‑based). */
    idx_t row = 1;
    idx_t acc = 1;           /* number of terms up to current row */
    while (acc < n) {
        ++row;
        acc += row;
    }

    /* Column within the row (1‑based). */
    idx_t prev_terms = (row - 1) * row / 2;
    idx_t col = n - prev_terms;   /* 1‑based column */

    return T[row][col];
}
