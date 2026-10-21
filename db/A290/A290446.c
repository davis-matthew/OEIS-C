/*
 * A290446  a(n) = Bell(n)*Motzkin(n-1).
 * https://oeis.org/A290446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) * Motzkin(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   motzkin_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/motzkin_upto.h"

/* maximum n that is expected to be queried (>= 10000) */
#define MAX_N 20000

/* static storage for Bell numbers up to MAX_N */
static seq_t bell[MAX_N + 1];
static idx_t bell_max = 0;          /* largest index for which bell[] is computed */

/* ensure that bell[0] is initialized (Bell(0) = 1) */
static void init_bell(void)
{
    if (bell_max == 0) {
        bell[0] = (seq_t)1;
        bell_max = 0;
    }
}

/* compute Bell numbers up to index n (inclusive), extending the static array */
static void compute_bell_upto(idx_t n)
{
    init_bell();

    if (n <= bell_max) return;          /* already available */

    for (idx_t i = bell_max + 1; i <= n; ++i) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binom(i-1,0) */

        idx_t m = i - 1;
        for (idx_t k = 0; k <= m; ++k) {
            sum += binom * bell[k];
            if (k < m) {
                /* binom(i-1,k+1) = binom(i-1,k) * (m-k) / (k+1) */
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }
    bell_max = n;
}

/* required exported function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    compute_bell_upto(n);           /* ensures bell[n] is valid */
    seq_t bell_n   = bell[n];
    seq_t motz     = motzkin_upto(n - 1);

    return bell_n * motz;
}
