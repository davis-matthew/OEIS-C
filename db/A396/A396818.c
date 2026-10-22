/*
 * A396818  Total number of separating two-component spanning forests over all
 *          unordered pairs of vertices in the wheel graph W_n.
 * https://oeis.org/A396818
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = m*((L_{2m}-2)*(F_{2q+1}-1) - F_{2m}*(L_{2q+1}-2q-1))
 *              - e_m*m/2*(F_m*(L_{2m}-2) - F_{2m}*(L_m-2)) + m*F_{2m},
 *          where m = n-1, q = floor(m/2), e_m = (1+(-1)^m)/2.
 *
 *  F_k: k‑th Fibonacci number,  F_0 = 0, F_1 = 1.
 *  L_k: k‑th Lucas number,  L_k = F_{k-1} + F_{k+1}  (L_0 = 2, L_1 = 1).
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- Fibonacci cache ---------- */
#define MAX_FIB 40005                 /* enough for n up to about 20000       */
static seq_t fib[MAX_FIB];
static size_t fib_size = 0;          /* number of valid entries in fib[0..]  */

static void ensure_fib(size_t up_to) /* include up_to in the cached range    */
{
    if (up_to < fib_size) return;
    if (fib_size == 0) {            /* initialise first two Fibonacci        */
        fib[0] = (seq_t)0;
        fib[1] = (seq_t)1;
        fib_size = 2;
    }
    for (size_t i = fib_size; i <= up_to; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    fib_size = up_to + 1;
}

/* ---------- Lucas accessor ---------- */
static seq_t lucas(idx_t k)
{
    if (k == 0) return (seq_t)2;
    if (k == 1) return (seq_t)1;
    /* requires F_{k-1} and F_{k+1}                         */
    ensure_fib((size_t)k + 1);
    return fib[k-1] + fib[k+1];
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 4U) return (seq_t)0;   /* undefined below offset */
    idx_t m = n - 1U;
    idx_t q = m >> 1U;             /* floor(m/2)            */

    /* Fibonacci numbers needed up to 2m+1                       */
    ensure_fib((size_t)(2*m + 1));

    seq_t F_2m  = fib[2*m];
    seq_t L_2m  = lucas(2*m);

    seq_t F_2q1 = fib[2*q + 1];
    seq_t L_2q1 = lucas(2*q + 1);

    seq_t F_m   = fib[m];
    seq_t L_m   = lucas(m);

    seq_t e_m = (m % 2U == 0U) ? (seq_t)1 : (seq_t)0;  /* even m → 1, else 0 */

    /* Subexpression 1 */
    seq_t term1 = L_2m - (seq_t)2;
    seq_t term2 = F_2q1 - (seq_t)1;
    seq_t term3 = L_2q1 - ( ((seq_t)2 * q) + (seq_t)1 );
    seq_t sub1  = term1 * term2 - F_2m * term3;
    seq_t part1 = m * sub1;

    /* Subexpression 2 */
    seq_t part2 = 0;
    if (e_m) {
        seq_t diff = F_m * (L_2m - (seq_t)2) - F_2m * (L_m - (seq_t)2);
        seq_t mul  = (e_m * m) / 2;          /* m is even when e_m==1   */
        part2 = mul * diff;
    }

    /* Part 3 */
    seq_t part3 = m * F_2m;

    seq_t result = part1 - part2 + part3;
    return result;
}
