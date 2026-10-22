/*
 * A363429  Number of set partitions of [n] such that each block has at most
 *          one even element.
 * https://oeis.org/A363429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..ceil(n/2)} floor(n/2)^k * binomial(ceil(n/2),
 *            k) * Bell(ceil(n/2)-k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *bell = NULL;
static idx_t bell_len = 0;
static idx_t bell_cap = 0;

/* Ensure Bell numbers up to index m are computed. */
static void ensure_bell(idx_t m)
{
    /* Allocate more space if needed. */
    if (bell_cap <= m) {
        idx_t new_cap = m + 1;
        void *ptr = realloc(bell, new_cap * sizeof(seq_t));
        if (!ptr) exit(1);
        bell = (seq_t *)ptr;
#ifdef __cplusplus
        for (idx_t i = bell_cap; i < new_cap; ++i)
            new (&bell[i]) seq_t();
#endif
        bell_cap = new_cap;
    }

    /* Compute any missing Bell numbers up to m. */
    for (idx_t i = bell_len; i <= m; ++i) {
        if (i == 0) {
            bell[0] = 1; /* Bell(0) = 1 */
            continue;
        }
        seq_t sum  = 0;
        seq_t comb = 1; /* C(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            /* term = Bell(k) * C(i-1, k) */
            seq_t term = bell[k];
            term = term * comb;
            sum += term;

            /* Update comb to C(i-1, k+1) if needed. */
            if (k + 1 < i) {
                comb = comb * (seq_t)(i - 1 - k);
                comb = comb / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }
    bell_len = m + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t e = n / 2;            /* floor(n/2)   */
    idx_t m = (n + 1) / 2;      /* ceil(n/2)    */

    /* Ensure we have Bell numbers up to m. */
    ensure_bell(m);

    seq_t result = 0;
    seq_t power  = 1;           /* e^k, starts at k = 0 */
    seq_t comb   = 1;           /* C(m,0) */

    for (idx_t k = 0; k <= m; ++k) {
        /* term = e^k * C(m,k) * Bell(m-k) */
        seq_t term = power;
        term = term * comb;
        term = term * bell[m - k];
        result += term;

        /* Update power and comb for next k. */
        if (k < m) {
            power = power * (seq_t)e;
            comb  = comb * (seq_t)(m - k);
            comb  = comb / (seq_t)(k + 1);
        }
    }
    return result;
}
