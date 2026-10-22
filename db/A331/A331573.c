/*
 * A331573  The bottom entry in the forward difference table of the Euler
 *          totient function phi for 1..n.
 * https://oeis.org/A331573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k)*binomial(n-1,k-1)*phi(k)
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 30, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where phi is Euler's totient function.                            */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- Ensure phi(k) values for 1..n are available -------------------------------- */
    static uint64_t *phi64 = NULL;   /* temporary 64‑bit storage for phi */
    static size_t phi_cap = 0;       /* current capacity of phi64 */

    if (n >= phi_cap) {
        size_t new_cap = (size_t)n + 1;          /* indices 0..n */
        uint64_t *tmp = (uint64_t*)realloc(phi64, new_cap * sizeof(uint64_t));
        if (!tmp) return 0;                      /* allocation failure */
        phi64 = tmp;
        for (size_t i = 1; i < new_cap; ++i) phi64[i] = i;

        for (size_t i = 2; i < new_cap; ++i) {
            if (phi64[i] == i) {                 /* i is prime */
                for (size_t j = i; j < new_cap; j += i)
                    phi64[j] -= phi64[j] / i;
            }
        }
        phi_cap = new_cap;
    }

    /* --- Compute a(n) using the forward‑difference formula -------------------------- */
    seq_t sum   = 0;                /* accumulator for the alternating sum */
    seq_t binom = 1;                /* binomial(n‑1,0) */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t diff = n - k;
        seq_t sign = ((diff & 1ULL) ? (seq_t)-1 : (seq_t)1);   /* (-1)^(n‑k) */

        seq_t phi_k = (seq_t)phi64[(size_t)k];
        seq_t term  = binom * phi_k;
        if (sign == (seq_t)-1) term = -term;
        sum += term;

        if (k != n) {
            /* binom = binom * (n‑k) / k  (exact division) */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
        }
    }
    return sum;
}
